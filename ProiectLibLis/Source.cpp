#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include "Operatii_liste.h"

using namespace sf;
using namespace std;

#define FRAME_INTERVAL 20
#define WIDTH_SCREEN 1500
#define HEIGHT_SCREEN 800

ContextSettings settings;
Font font;
Clock myClock;

class Element;
class Node;
class Buton;

vector < Element* > Elements;
vector < Node* > Nodes;

vector < Node* > NodesSLL;
vector < Node* > NodesDLL;
vector < Node* > NodesStack;
vector < Node* > NodesQueue;

unordered_map < string, Buton* > ButonDictionar;

queue < pair < string, string > > customEvents;
/// optiunea de structura de date aleasa de utilizator
string optionForDS;

/// calculeaza distanta dintre doua puncte la puterea a 2 a
int DistanceBetweenTwoPointsSquared(Vector2f point1, Vector2f point2)
{
    return (point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y);
}

bool isPositiveNumber(const string& s)
{
    for (char const& ch : s) {
        if (isdigit(ch) == 0)
            return false;
    }
    return true;
}

float distanceBetweenTwoPoints(Vector2f point1, Vector2f point2) {
    return sqrtf(powf((point2.x - point1.x), 2) + powf((point2.y - point1.y), 2));
}

Vector2f multiplyVector2f(Vector2f a, Vector2f b)
{
    return Vector2f(a.x * b.x, a.y * b.y);
}
Vector2f divideVector2f(Vector2f a, Vector2f b)
{
    return Vector2f(a.x / b.x, a.y / b.y);
}

/// sageata cu partea ascutita spre p2
ConvexShape getArrow(Vector2f p1, Vector2f p2 , string part = "line" , Vector2f width = { 4,4 } , Vector2f varf_percent = Vector2f(10, 10) , Vector2f padding = { 5,5 })
{
    ConvexShape convex;
    convex.setPointCount(4);
    /*
    Vector2f p1 = Vector2f(500, 400);
    Vector2f p2 = Vector2f(500, 300);
    */

    Vector2f right = Vector2f(1, -1);
    Vector2f left = Vector2f(-1, 1);
    Vector2f dif = (p2 - p1);
    Vector2f step = divideVector2f(dif, { 100,100 });
    Vector2f perp_step1 = multiplyVector2f({ step.y, step.x }, right);
    //Vector2f varf_percent = Vector2f(10, 10);
    Vector2f varf_ascutit = Vector2f(20, 20);
    Vector2f t1 = multiplyVector2f(perp_step1, varf_percent) + p2 - multiplyVector2f(step, varf_ascutit);
    Vector2f perp_step2 = multiplyVector2f({ step.y, step.x }, left);
    Vector2f t2 = multiplyVector2f(perp_step2, varf_percent) + p2 - multiplyVector2f(step, varf_ascutit);
    Vector2f almost_p2 = p1 + multiplyVector2f(step, Vector2f{ 100,100 } - varf_ascutit );
    Vector2f almost_p2_t1 = almost_p2 + perp_step1;
    Vector2f almost_p2_t2 = almost_p2 + perp_step2;


    convex.setPoint(0, almost_p2);
    convex.setPoint(1, t1);
    convex.setPoint(2, p2);
    convex.setPoint(3, t2);
    convex.setFillColor(Color::Black);

    /// partea ascutita
    if (part == "sharp") return convex;

    ConvexShape convex2;
    convex2.setPointCount(4);
    convex2.setPoint(0, p1 + multiplyVector2f(perp_step1, width) + multiplyVector2f(step, padding));
    convex2.setPoint(3, p2 + multiplyVector2f(perp_step1, width) - multiplyVector2f(step, padding));
    convex2.setPoint(1, p1 + multiplyVector2f(perp_step2, width) + multiplyVector2f(step, padding));
    convex2.setPoint(2, p2 + multiplyVector2f(perp_step2, width) - multiplyVector2f(step, padding));
    /*

    convex2.setPoint(0, p1 + multiplyVector2f(right, width) + multiplyVector2f(step, padding));
    convex2.setPoint(3, p2 + multiplyVector2f(right, width) - multiplyVector2f(step, padding));
    convex2.setPoint(1, p1 + multiplyVector2f(left, width) + multiplyVector2f(step, padding));
    convex2.setPoint(2, p2 + multiplyVector2f(left, width) - multiplyVector2f(step, padding));
    */
    convex2.setFillColor(Color::Black);

    if (part == "line") return convex2;
    return convex2;
}



class Element : public Drawable, public Transformable
{
protected:
    int priority;
    string _id;
    Vector2f position;
    Color _color;

public:
    string type, ti_text;
    bool isInteractive, isDisplayed = true, ti_focused;
    CircleShape circle;
    RectangleShape rectangle;
    //   Shape* shape; IN LOC DE CIRCLE SI RECTANGLE SE POATE FOLOSI *shape CA ABSTRACTIZARE
    vector < Element* > Children;
    virtual void setPosition(Vector2f point) = 0;
    virtual Vector2f getPosition() = 0;
    virtual void move(Vector2f direction) = 0;
    virtual void draw(RenderTarget& target, RenderStates states) const = 0;
    virtual void onClicked() = 0;
    virtual bool isInRange(Vector2f point) = 0;
    virtual void setColor(Color Color) {};
    virtual void onHover() {};
    virtual void setTexture(Texture* texture) {};
    virtual void makeVisible() {};
    virtual void makeInvisible() {};
    virtual void handleTextInput(string input) {};
    const string getText() {};
    virtual void updateTextPosition() {};

};

class Node : public Element
{
private:
    Text text;
    Text below_text;
    string info;

    void updateTextPosition()
    {
        text.setPosition({ position.x - (text.getLocalBounds().width / 2) - 0, position.y - (text.getLocalBounds().height / 2) - 10 });
        below_text.setPosition({ position.x - (below_text.getLocalBounds().width / 2) - 0, position.y - (below_text.getLocalBounds().height / 2) + 30 });
    }

public:
    ///  constructorul pentru nod
    Node(Vector2f point = { 500,500 }, int radius = 100, string value = "", string id = "", string ds_type = "sll", Color color = Color::Red, bool interactive = true)
    {
        isInteractive = interactive;

        type = "Node";
        position = point;
        info = value;
        _id = id;

        text.setString(value);
        text.setFont(font);
        
        below_text.setFillColor(Color::Black);

        updateTextPosition();

        circle.setRadius(radius);
        setCenterCoordinates(position);
        circle.setFillColor(color);

        Nodes.push_back(this);
       // Elements.push_back(this);
        /*
        if (ds_type == "sll") NodesSLL.push_back(this);
        else if (ds_type == "dll") NodesDLL.push_back(this);
        else if (ds_type == "s") NodesStack.push_back(this);
        else if (ds_type == "q") NodesQueue.push_back(this);
        */
    }

    void setPosition(Vector2f point)
    {
        setCenterCoordinates(point);
    }

    void setCenterCoordinates(Vector2f point)
    {
        position = point;
        circle.setPosition(point.x - circle.getRadius(), point.y - circle.getRadius());
        updateTextPosition();
    }

    bool isInRange(Vector2f point)
    {
        if (!isInteractive) return false;
        int raza = circle.getRadius();
        return DistanceBetweenTwoPointsSquared(position, point) < (raza * raza);
    }

    /// metoda ce este apelata automat cand dau click pe un nod
    void onClicked()
    {
        cout << "AM DAT CLICK PE NOD" << endl;
    }

    void onHover(Vector2f point)
    {
        cout << "ON HOVER" << endl;
    }

    static Node* getRandomNode()
    {
        // factory method
    }

    /// metoda ce este chemata automat atunci cand elementul este desenat
    void draw(RenderTarget& target, RenderStates states) const
    {
        if (isDisplayed)
        {
            target.draw(circle, states);
            target.draw(text, states);
            target.draw(below_text, states);
        }
    }

    /// ca sa schimbi culoarea nodului
    void setColor(Color color)
    {
        circle.setFillColor(color);
    }

    Vector2f getPosition()
    {
        return position;
    }

    float getRadius()
    {
        return circle.getRadius();
    }

    void move(Vector2f direction)
    {
        setPosition(position + direction);
    }

 
    void setTextBelow(string str)
    {
        below_text.setString(str);
        below_text.setFont(font);
        updateTextPosition();
    }
    void setTextBelowColor(Color color)
    {
        below_text.setFillColor(color);
    }
};

/// Te folosesti de coada asta pentru a face animatii
queue < pair < Node*, Vector2f> > requestForAnimation;

class Buton : public Element
{
private:

    Text text;
    string info;
    int text_width, text_height;

public:
    /// constructorul pentru buton
    Buton(Vector2f point, Vector2f size = { 50,100 }, string value = "", string id = "", Color color = Color::Blue, bool interactive = true)
    {
        // position=point;
        if (id.find("ti_") == 0)
            type = "TextField";
        else
            type = "Buton";
        _id = id;
        isInteractive = interactive;
        _color = color;
        
        if (id != "sllBtn" && id != "dllBtn" && id != "stackBtn" && id != "queueBtn") {
            makeInvisible();
        }
        
        
        rectangle.setSize(size);
        rectangle.setPosition(point);
        if (id.find("ti_") == 0) {
            rectangle.setFillColor(Color::White);
            rectangle.setOutlineColor(color);
            rectangle.setOutlineThickness(1);
            text.setFillColor(Color::Red);
        }
        else
            rectangle.setFillColor(color);

        text.setString(value);
        text.setFont(font);
        text_width = text.getLocalBounds().width;
        text_height = text.getLocalBounds().height;
        updateTextPosition();

        Elements.push_back(this);
        //ButonDictionar[_id] = this;
    }
    void handleTextInput(string input) {
        string str = text.getString();
        text.setString(input);
        if (rectangle.getSize().x < text.getLocalBounds().width + 10)
            handleTextInput(str);
        updateTextPosition();
    }
    const string getText() const {
        return text.getString();
    }

    // cu asta recalculez pozitia textului din element
    void updateTextPosition()
    {
        text_width = text.getLocalBounds().width;
        text_height = text.getLocalBounds().height;
        float padding_left = (rectangle.getSize().x - text_width) / 2;
        float padding_top = (rectangle.getSize().y - text_height) / 2;
        text.setPosition(rectangle.getPosition() + Vector2f({ padding_left,padding_top }) + Vector2f({ -3,-6 }));
    }

    void setText(string str)
    {
        text.setString(str);
        updateTextPosition();
    }

    void setTexture(Texture* texture)
    {

    }
    void onHover(Vector2f point)
    {

    }
    /// setez culoarea butonului
    void setColor(Color color)
    {
        rectangle.setFillColor(color);
    }

    /// metoda ce este chemata automat cand apas pe un buton
    void onClicked()
    {
        ti_focused = false;
        if (_id.find("ti_") == 0) {
            ti_focused = true;
        }
        else {
            setColor(Color::White);
            customEvents.push({ _id, "click" });

            ///opresc timpul ca sa creez efectul de click pe buton ca sa se vada noua culoare
            sleep(milliseconds(40));

            /// pun culoarea inapoi dupa trecerea timpului
            setColor(_color);
        }
    }

    /// functie asta decide daca punctul primit ca parametru este pe element ( punctul e folosit ca mouse cel mai probabil )
    bool isInRange(Vector2f point)
    {
        if (!isInteractive) return false;
        return rectangle.getGlobalBounds().contains(point);
    }

    void draw(RenderTarget& target, RenderStates states) const
    {


        if (isDisplayed)
        {
            target.draw(rectangle, states);
            target.draw(text, states);
        }
    }
    Vector2f getPosition()
    {
        return { rectangle.getPosition().x,rectangle.getPosition().y };
    }
    void setPosition(Vector2f point)
    {
        rectangle.setPosition(point);
        updateTextPosition();
    }
    void move(Vector2f direction)
    {
        setPosition(position + direction);
        updateTextPosition();
    }
    void toString()
    {

    }

    void makeVisible()
    {
        isDisplayed = true;
        isInteractive = true;
    }

    void makeInvisible()
    {
        isDisplayed = false;
        isInteractive = false;
    }

};

/// functie ce se ocupa de toate animatiile
void animations()
{
    struct animation
    {
        Node* elem;
        Vector2f destination;
        int percent = 0;

    } anim;
    vector < animation > Animations;

    Time time_interval = milliseconds(FRAME_INTERVAL);
    Time timeSnapshot = milliseconds(0);
    while (true)
    {
        if (myClock.getElapsedTime() - timeSnapshot > time_interval)
        {
            /// aici pot modifica timpul de incepere al unei animatii anume
            while (!requestForAnimation.empty())
            {
                Animations.push_back({ requestForAnimation.front().first,requestForAnimation.front().second,0 });
                requestForAnimation.pop();
            }

            if (!Animations.empty())
            {
                auto it = Animations.begin();
                while (it != Animations.end())
                {
                    if ((*it).percent == 100)
                    {
                        it = Animations.erase(it);
                    }
                    else
                    {
                        Vector2f diff = ((*it).destination - (*it).elem->getPosition());
                        (*it).elem->move((diff / float(100)) * float(((*it).percent)));
                        (*it).percent++;
                        ++it;
                    }
                }
            }

            timeSnapshot = myClock.getElapsedTime();
        }
    }
}

float distanceBetweenTwoPoints(Vector2f point1, Vector2f point2);
/// functie ce este apelata automat ce da refresh la ecran
void updateScreen(RenderWindow* window)
{
    Context context;
    Time time_interval = milliseconds(FRAME_INTERVAL);
    Time timeSnapshot = milliseconds(0);

    while (window->isOpen())
    {
        if (myClock.getElapsedTime() - timeSnapshot > time_interval)
        {
            window->clear(Color::White);
            
            try
            {
                // aici desenez legaturile dintre noduri
                Node* last = nullptr;
                int counter = 0;

                /// asta e pentru evitarea erorii de accesare a unei memorii din diferite thread uri
                vector < Node* > copyNodesSLL;
                copyNodesSLL = NodesSLL;


                for (auto element : copyNodesSLL)
                {
                    if (last != nullptr) {
                        Vector2f width = { 1,1 };
                        Vector2f arrowSize = { 8,8 };
                        Vector2f pos1 = (*last).getPosition(), pos2 = (*element).getPosition();
                        float radius = (*element).getRadius();
                        Vector2f orientation1 = {radius,0};
                        Vector2f orientation2 = {-radius,0 };
                        if (counter % 3 == 0) {
                            width = { 0.5,0.5 }; 
                            orientation1 = { -radius ,radius };
                            orientation2 = { radius ,-radius };
                            arrowSize = { 5 ,5 };
                        }

                        float dist = distanceBetweenTwoPoints(pos1, pos2);
                        /// de terminat
                       if (dist < 230.0)
                        {
                           ConvexShape ar = getArrow(pos1 + orientation1, pos2 + orientation2, "line", width);
                           ConvexShape ar2 = getArrow(pos1 + orientation1, pos2 + orientation2, "sharp", width, arrowSize);
                           //ConvexShape ar3 = getArrow(pos2 + orientation2, pos1 + orientation1, "sharp", width, arrowSize);

                            window->draw(ar);
                            window->draw(ar2);
                           // window->draw(ar3);
                        }
                        
                    }
                    counter++;
                    last = element;
                    //window->draw(*element);
                }
                last = nullptr;
                counter = 0;

                for (auto element : NodesDLL)
                {
                    if (last != nullptr) {
                        Vector2f width = { 1,1 };
                        Vector2f arrowSize = { 10,10 };
                        Vector2f pos1 = (*last).getPosition(), pos2 = (*element).getPosition();
                        float radius = (*element).getRadius();
                        Vector2f orientation1 = { 0,radius };
                        Vector2f orientation2 = { 0,-radius };
                        float dist = distanceBetweenTwoPoints(pos1, pos2);
                        /// de terminat
                       if( dist < 200.0 )
                        {
                            ConvexShape ar = getArrow(pos1 + orientation1, pos2 + orientation2, "line", width);
                            ConvexShape ar2 = getArrow(pos1 + orientation1, pos2 + orientation2, "sharp", width, arrowSize);
                            ConvexShape ar3 = getArrow(pos2 + orientation2, pos1 + orientation1, "sharp", width, arrowSize);

                            window->draw(ar);
                            window->draw(ar2);
                            window->draw(ar3);
                        }
                    }
                    counter++;
                    last = element;
                    //window->draw(*element);
                }
                for (auto element : NodesStack)
                {
                    //window->draw(*element);
                }
                for (auto element : NodesQueue)
                {
                    //window->draw(*element);
                }


                /// aici desenez nodurile
                for (auto element : NodesSLL)
                {
                    window->draw(*element);
                }
                for (auto element : NodesDLL)
                {
                    window->draw(*element);
                }
                for (auto element : NodesStack)
                {
                    //window->draw(*element);
                }
                for (auto element : NodesQueue)
                {
                    //window->draw(*element);
                }

                /// aici desenez butoanele
                for (auto element : Elements)
                {
                    window->draw(*element);
                }

            }
            catch (const std::exception&)
            {

            }
            window->display();
            timeSnapshot = myClock.getElapsedTime();
            //myClock.restart();
        }
    }

}

/// TODO: DE TERMINAT ASTA
class DataStructureVisualizer {
private:
    vector <Node*>* Vector;
    Vector2f corner_position;
    Vector2f nodeSpawn_position;
    int columns = 3;
    string ds_type;
    List list;
    Time speed = seconds(0.5);

    void update()
    {
        /// aici recalculam pozitia fiecarui nod , legaturile si o sa facem requestForAnimation.push({ nod, new_target }); pentru fiecare nod parcurgand vectorul cu formula de target cu cateva randuri mai jos
        int counter = 0;
        for (auto node : *Vector)
        {
            Vector2f target = Vector2f(corner_position.x + 100 * (counter % columns), corner_position.y + 100 * (counter / columns));
            counter++;
            requestForAnimation.push({ node, target });
        }
    }
public:
    DataStructureVisualizer(Vector2f corn_pos, int col, vector <Node*>* vec, string type_ds = "sll", Vector2f spawn_pos = { 100,100 })
    {
        nodeSpawn_position = spawn_pos;
        corner_position = corn_pos;
        Vector = vec;
        columns = col;
        ds_type = type_ds;
    };
    void pushNode(string val , int position)
    {
        //int counter = (*Vector).size();
        Node* nod = new Node(nodeSpawn_position, 25, val , "node" + ds_type + val, ds_type);
        (*Vector).insert((*Vector).begin() + position,nod);
        sleep(speed);
        update();
    }
    void pushBack(string val)
    {
        int counter = (*Vector).size();
        Node* nod = new Node(nodeSpawn_position, 25, val, "node" + ds_type + val, ds_type);
        (*Vector).insert((*Vector).begin() + counter, nod);
        sleep(speed);
        update();
    }
    void pushFront(string val)
    {
        Node* nod = new Node(nodeSpawn_position, 25, val, "node" + ds_type + val, ds_type);
        (*Vector).insert((*Vector).begin() , nod);
        sleep(speed);
        update();
    }

    void popBack()
    {
        (*Vector).back()->setColor(Color::Green);
        (*Vector).back()->setTextBelow("back");
        sleep(speed);
        (*Vector).back()->setTextBelow("delete");
        (*Vector).back()->setTextBelowColor(Color::Red);
        sleep(speed);
        (*Vector).pop_back();
        update();
    }

    void popFront()
    {
        (*Vector).front()->setColor(Color::Green);
        (*Vector).front()->setTextBelow("front");
        sleep(speed);
        (*Vector).front()->setTextBelow("delete");
        (*Vector).front()->setTextBelowColor(Color::Red);
        sleep(speed);
        (*Vector).erase((*Vector).begin());
        update();
    }

    void deleteNode(int counter)
    {
        for(int i=0 ; i <= counter ; i++)
        {
            (*Vector)[i]->setColor(Color::Blue);
            (*Vector)[i]->setTextBelow("iter");
            sleep(speed);
            if (i < counter) 
            { 
                (*Vector)[i]->setTextBelow("index < " +to_string(counter));
                sleep(speed);
                (*Vector)[i]->setTextBelow("iter = iter -> next");
                sleep(speed); 
            }
            (*Vector)[i]->setColor(Color::Red);
            (*Vector)[i]->setTextBelow("");
        }
        (*Vector)[counter]->setTextBelow("index == " + to_string(counter));
        sleep(speed);
        (*Vector)[counter]->setTextBelow("delete");
        (*Vector)[counter]->setColor(Color::Green);
        (*Vector)[counter]->setTextBelowColor(Color::Blue);
        sleep(speed);
        (*Vector).erase((*Vector).begin()+counter);
        update();
    }

    void deleteNode(Node* nod)
    {
        //Nodes.erase(nod);
        sleep(speed);
        update();
    }

    void erase()
    {
        for (auto node : (*Vector))
        {
            requestForAnimation.push({ node,{-100,1500} });
        }
        // nu stiu de ce nu merge *
        sleep(speed);
        (*Vector).clear();

    }

};

DataStructureVisualizer SLL({ 100,300 }, 3, &NodesSLL);
DataStructureVisualizer DLL({ 550,300 }, 1, &NodesDLL, "dll");
DataStructureVisualizer S({ 850,300 }, 1, &NodesStack, "s");
DataStructureVisualizer Q({ 1100,300 }, 1, &NodesQueue, "q");


void resolveCustomEvents()
{
    string id, event;
    while (!customEvents.empty())
    {
        id = customEvents.front().first;
        event = customEvents.front().second;
        customEvents.pop();
        if (id == "newListBtn")
        {
            if (event == "click")
            {
                if (optionForDS == "SLL") SLL.erase();
            }
        }
        else if (id == "addNodeBtn")
        {
            if (event == "click")
            {
                //animateNewNode();
                string val, pos;
                val = ButonDictionar["ti_addNodeData"]->getText();
                pos = ButonDictionar["ti_addNodePos"]->getText();
                if (optionForDS == "SLL")
                {

                    if (isPositiveNumber(pos) && val != "Value") {
                        SLL.pushNode(val, stoi(pos));
                    }
                    else if (val != "Value")
                    {
                        SLL.pushBack(val);

                    }
                    else if (isPositiveNumber(pos))
                    {
                        SLL.pushNode(to_string(NodesSLL.size()),stoi(pos));
                    }
                    else
                    {
                        SLL.pushBack(to_string(NodesSLL.size()));
                    }
                }
                //if (optionForDS == "SLL") SLL.pushNode(to_string(NodesSLL.size()), NodesSLL.size());
                else  if (optionForDS == "DLL") DLL.pushNode(to_string(NodesDLL.size()),NodesDLL.size());
                ButonDictionar["ti_addNodeData"]->handleTextInput("");
                ButonDictionar["ti_addNodePos"]->handleTextInput("");
                ButonDictionar["ti_addNodeData"]->setText("Value");
                ButonDictionar["ti_addNodePos"]->setText("Position");

            }
        }
        else if (id == "pushNodeBtn")
        {
            if (event == "click")
            {
           // if (optionForDS == "S") S.pushNode(to_string(NodesStack.size()));
           // else  if (optionForDS == "Q") Q.pushNode(to_string(NodesQueue.size()));
            }
        }
        else if (id == "delNodeBtn")
        {
            if (event == "click")
            {
                string val, pos;
                val = ButonDictionar["ti_addNodeData"]->getText();
                pos = ButonDictionar["ti_addNodePos"]->getText();

                if (optionForDS == "SLL")
                {
                    if (isPositiveNumber(pos)) {
                        SLL.deleteNode(stoi(pos));
                    }
                    else
                    {
                        SLL.popBack();
                    }
                }
                else if (optionForDS == "DLL") DLL.deleteNode(4);
                ButonDictionar["ti_addNodeData"]->handleTextInput("");
                ButonDictionar["ti_addNodePos"]->handleTextInput("");
                ButonDictionar["ti_addNodeData"]->setText("Value");
                ButonDictionar["ti_addNodePos"]->setText("Position");
            }
        }
        else if (id == "sllBtn")
        {
            if (event == "click")
            {
                optionForDS = "SLL";
                ButonDictionar["newListBtn"]->makeVisible();
                ButonDictionar["addNodeBtn"]->makeVisible();
                ButonDictionar["delNodeBtn"]->makeVisible();
                ButonDictionar["ti_addNodePos"]->makeVisible();
                ButonDictionar["ti_addNodeData"]->makeVisible();
                ButonDictionar["popNodeBtn"]->makeInvisible();
                ButonDictionar["pushNodeBtn"]->makeInvisible();




            }
        }
        else if (id == "dllBtn")
        {
            if (event == "click")
            {
                optionForDS = "DLL";
                ButonDictionar["newListBtn"]->makeVisible();
                ButonDictionar["addNodeBtn"]->makeVisible();
                ButonDictionar["delNodeBtn"]->makeVisible();
                ButonDictionar["pushNodeBtn"]->makeInvisible();
                ButonDictionar["popNodeBtn"]->makeInvisible();
            }
        }
        else if (id == "stackBtn")
        {
            if (event == "click")
            {
                optionForDS = "S";
                ButonDictionar["newListBtn"]->makeVisible();
                ButonDictionar["addNodeBtn"]->makeInvisible();
                ButonDictionar["delNodeBtn"]->makeInvisible();
                ButonDictionar["pushNodeBtn"]->makeVisible();
                ButonDictionar["popNodeBtn"]->makeVisible();
            }
        }
        else if (id == "queueBtn")
        {
            if (event == "click")
            {
                optionForDS = "Q";
                ButonDictionar["newListBtn"]->makeVisible();
                ButonDictionar["addNodeBtn"]->makeInvisible();
                ButonDictionar["delNodeBtn"]->makeInvisible();
                ButonDictionar["pushNodeBtn"]->makeVisible();
                ButonDictionar["popNodeBtn"]->makeVisible();
            }
        }
    }
}


int main()
{
    font.loadFromFile("./Fonts/Montserrat-Regular.ttf");
    RenderWindow window(VideoMode(WIDTH_SCREEN, HEIGHT_SCREEN), "SFML works!", Style::Close, settings);
    window.setActive(false);
    Thread updateScreenThread(&updateScreen, &window);
    Thread animationsThread(&animations);
    updateScreenThread.launch();
    animationsThread.launch();
    /*SoundBuffer buffer;
    if (!buffer.loadFromFile("sound.wav"))
        return -1;
    Sound sound;
    sound.setBuffer(buffer);
    sound.play();*/


    /// IN LOC SA CREEZ VARIABILE MAI BINE LE PUN IN DICTIONAR DIRECT
    ButonDictionar["sllBtn"] =new Buton({50,0}, {300,50}, "Singly Linked List", "sllBtn");
    ButonDictionar["dllBtn"] = new Buton({ 400,0 }, { 300,50 }, "Doubly Linked List", "dllBtn");
    ButonDictionar["stackBtn"] = new Buton({ 750,0 }, { 200,50 }, "Stack", "stackBtn");
    ButonDictionar["queueBtn"] = new Buton({ 1000,0 }, { 200,50 }, "Queue", "queueBtn");
    ButonDictionar["newListBtn"] = new Buton({ 1200,200 }, { 300,50 }, "New List", "newListBtn");
    ButonDictionar["addNodeBtn"] = new Buton({ 1200,400 }, { 140,50 }, "Add", "addNodeBtn");
    ButonDictionar["delNodeBtn"] = new Buton({ 1350,400 }, { 150,50 }, "Delete", "delNodeBtn");
    ButonDictionar["pushNodeBtn"] = new Buton({ 1200,500 }, { 300,50 }, "Push node", "pushNodeBtn");
    ButonDictionar["popNodeBtn"] = new Buton({ 1200,600 }, { 300,50 }, "Pop node", "popNodeBtn");
    ButonDictionar["clearListBtn"] = new Buton( {1200,700},{300,50},"Clear List","clearListBtn" );

    ButonDictionar["ti_addNodePos"] = new Buton({ 1200,300 }, { 140,50 }, "Position", "ti_addNodePos");
    ButonDictionar["ti_addNodeData"] = new Buton({ 1350,300 }, { 150,50 }, "Value", "ti_addNodeData");



    //ButonDictionar["test"] = new Buton({ 500,500 }, { 300,50 }, "test", "test");
    
    Vector2f mousePosition;
    Element* target = nullptr;
    bool press = false;
    string ti_input;

    while (window.isOpen())
    {
        /// aici rezolv logica interfetei
        resolveCustomEvents();
        /// prin event se refera la interactiunea cu mouse ul , tastatura etc
        Event event;
        while (window.pollEvent(event))
        {
            /// target este singurul element care este atins de mouse in acel timp ( asa stiu usor pe ce element s a facut click sau hover )
            target = nullptr;
            mousePosition = Vector2f(Mouse::getPosition(window));
            for (auto element : Elements)
            {
                if (element->isInRange(mousePosition))
                {
                    target = element;
                    target->onHover();
                }
            }
            if (target == nullptr)
            {
                for (auto element : Nodes)
                {
                    if (element->isInRange(mousePosition))
                    {
                        target = element;
                        target->onHover();
                    }
                }
            }

            if (event.type == Event::Closed)
                window.close();


            if (event.type == Event::MouseButtonPressed)
            {
                /// Click stanga
                if (event.mouseButton.button == Mouse::Left)
                {
                    press = true;
                    if (target != nullptr) target->onClicked();
                    //else cout<<"EROARE !!!! TARGET ESTE NULL"<<endl;
                }
                /// Click dreapta
                else if (event.mouseButton.button == Mouse::Right)
                {
                    // cout<<"AM APASAT DREAPTA"<<endl;
                }
            }
            /// am miscat mouse ul
            if (event.type == sf::Event::MouseMoved)
            {
                if (target != nullptr)
                {
                    /// asa pot sa trag nod urile cu mouse ul
                    if (press == true && target->type == "Node")
                    {
                        target->setPosition(mousePosition);
                    }
                }
            }
            //if (target->type == "TextField") {
            if (target != nullptr && target->ti_focused == true) {
                
                // target->updateTextPosition();
                if (event.type == Event::TextEntered) {
                    if (event.text.unicode >= 32 && event.text.unicode <= 127) {
                        ti_input += event.text.unicode;
                        target->handleTextInput(ti_input);
                    }
                }
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::BackSpace) {
                    if (!ti_input.empty()) {
                        ti_input.pop_back();
                        target->handleTextInput(ti_input);
                    }
                }
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Return) {
                    ti_input.clear();
                    target->handleTextInput(ti_input);
                }
            }
            if (target == nullptr || target->ti_focused == false)
                ti_input.clear();
            //}
        }
        /// cand am eliberat click ul
        if (event.type == Event::MouseButtonReleased)
        {
            press = false;
        }
    }
    return 0;
}