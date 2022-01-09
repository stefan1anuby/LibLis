#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include <mutex>
#include "Operatii_liste.h"

using namespace sf;
using namespace std;

#define FRAME_INTERVAL 15
#define WIDTH_SCREEN 1500
#define HEIGHT_SCREEN 800

mutex m;

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

unordered_map < string, unordered_map < string, string > > Language;
string limba = "en";

void words() {
    Language["en"]["sllBtn"] = "Singly Linked List";
    Language["en"]["dllBtn"] = "Doubly Linked List";
    Language["en"]["stackBtn"] = "Stack";
    Language["en"]["queueBtn"] = "Queue";

    Language["en"]["newListBtn"] = "New List";

    Language["en"]["addNodeBtn"] = "Add";
    Language["en"]["pushNodeBtn"] = "Push";
    Language["en"]["pushFrontBtn"] = "+Front";
    Language["en"]["pushBackBtn"] = "+Back";

    Language["en"]["delNodeBtn"] = "Delete";
    Language["en"]["popNodeBtn"] = "Pop";
    Language["en"]["popFrontBtn"] = "-Front";
    Language["en"]["popBackBtn"] = "-Back";

    Language["en"]["saveLists"] = "Save";
    Language["en"]["loadLists"] = "Load";

    Language["en"]["ti_addNodePos"] = "Position";
    Language["en"]["ti_addNodeData"] = "Value";

    Language["en"]["slowUp"] = "Slow up";
    Language["en"]["speedUp"] = "Speed up";

    Language["en"]["languageBtn"] = "en";

    Language["ro"]["sllBtn"] = "Lista Simplu Inlantuita";
    Language["ro"]["dllBtn"] = "Lista Dublu Inlantuita";
    Language["ro"]["stackBtn"] = "Stiva";
    Language["ro"]["queueBtn"] = "Coada";

    Language["ro"]["newListBtn"] = "Lista noua";

    Language["ro"]["addNodeBtn"] = "Adauga";
    Language["ro"]["pushNodeBtn"] = "Push";
    Language["ro"]["pushFrontBtn"] = "+Fata";
    Language["ro"]["pushBackBtn"] = "+Coada";

    Language["ro"]["delNodeBtn"] = "Sterge";
    Language["ro"]["popNodeBtn"] = "Pop";
    Language["ro"]["popFrontBtn"] = "-Fata";
    Language["ro"]["popBackBtn"] = "-Coada";

    Language["ro"]["saveLists"] = "Salvare";
    Language["ro"]["loadLists"] = "Incarcare";

    Language["ro"]["ti_addNodePos"] = "Pozitie";
    Language["ro"]["ti_addNodeData"] = "Valoare";

    Language["ro"]["slowUp"] = "Scade";
    Language["ro"]["speedUp"] = "Creste";

    Language["ro"]["languageBtn"] = "ro";
}



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
    Text text,below_text,onTop_text;
    string info;

    void updateTextPosition()
    {
        try
        {
            text.setPosition( (int) (position.x - (text.getLocalBounds().width / 2) - 0) , (int) (position.y - (text.getLocalBounds().height / 2) - 5 ) );
            below_text.setPosition((int) (position.x - (below_text.getLocalBounds().width / 2) - 0 ), (int) (position.y - (below_text.getLocalBounds().height / 2) + 30 ) );
            onTop_text.setPosition( (int)  (position.x - (onTop_text.getLocalBounds().width / 2) - 0 ), (int) (position.y - (onTop_text.getLocalBounds().height / 2) - 50 ) );

        }
        catch (const std::exception& e)
        {
            cout << e.what() << endl;
        }

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
        text.setCharacterSize(23);
        
        below_text.setFillColor(Color::Black);
        below_text.setCharacterSize(20);
        onTop_text.setFillColor(Color::Black);

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
        circle.setPosition((int) (point.x - circle.getRadius() ),(int) (point.y - circle.getRadius()) );
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
            target.draw(onTop_text, states);
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
        try
        {

            below_text.setString(str);
            below_text.setFont(font);
            updateTextPosition();
        }
        catch (const std::exception& e)
        {
            cout << e.what() << endl;
        }
    }
    void setTextBelowColor(Color color)
    {
        below_text.setFillColor(color);
    }
    void setTextOnTop(string str)
    {
        onTop_text.setString(str);
        onTop_text.setFont(font);
        updateTextPosition();
    }
    void setTextOnTopColor(Color color)
    {
        onTop_text.setFillColor(color);
    }

    void setBorder(float thickness, Color color)
    {
        circle.setOutlineThickness(thickness);
        circle.setOutlineColor(color);
    }
};

/// Te folosesti de coada asta pentru a face animatii
queue < pair < Node*, Vector2f> > requestForAnimation;

class Buton : public Element
{
private:

    Text text;
    string info;

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
        
        if (id != "sllBtn" && id != "dllBtn" && id != "stackBtn" && id != "queueBtn" && id != "saveListsBtn" && id != "loadListsBtn" && id != "languageBtn") {
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
        text.setCharacterSize(24);

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
        float text_width = text.getLocalBounds().width;
        float text_height = text.getLocalBounds().height;
        float padding_left = (rectangle.getSize().x - text_width) / 2;
        float padding_top = (rectangle.getSize().y - text_height) / 2;
        Vector2f position = rectangle.getPosition() + Vector2f({ padding_left,padding_top }) + Vector2f({0,-6}) ;
        text.setPosition(position);
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
            m.lock();
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
            m.unlock();

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
            //window->clear(Color::White);
            window->clear(Color(39.0 * 25, 39.0 * 25, 39.0 * 25 ));
            
            m.lock();

            try
            {
                // aici desenez legaturile dintre noduri
                Node* last = nullptr;
                int counter = 0;

                /// asta e pentru evitarea erorii de accesare a unei memorii din diferite thread uri
                vector < Node* > copyNodesSLL;
                vector < Node* > copyNodesDLL;
                vector < Node* > copyNodesStack;
                vector < Node* > copyNodesQueue;
                copyNodesSLL = NodesSLL;
                copyNodesDLL = NodesDLL;
                copyNodesStack = NodesStack;
                copyNodesQueue = NodesQueue;


                for (auto element : copyNodesSLL)
                {
                    if (counter == 0) {
                        (*element).setBorder(3, Color::Black);
                        (*element).setTextOnTop("head");
                    }
                    else if (counter == copyNodesSLL.size() - 1) {
                        (*element).setBorder(3, Color::Black);
                        (*element).setTextOnTop("tail");
                    }
                    else
                    {
                        (*element).setBorder(0, Color::Black);
                        (*element).setTextOnTop("");
                    }

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
                       if (dist < 225.0)
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

                for (auto element : copyNodesDLL)
                {
                    if (counter == 0) {
                        (*element).setBorder(3, Color::Black);
                        (*element).setTextOnTop("head");
                    }
                    else if (counter == copyNodesDLL.size() - 1) {
                       (*element).setBorder(3, Color::Black);
                       (*element).setTextOnTop("tail");
                    }
                    else
                    {
                        (*element).setBorder(0, Color::Black);
                        (*element).setTextOnTop("");
                    }

                    if (last != nullptr) {
                        Vector2f width = { 1,1 };
                        Vector2f arrowSize = { 10,10 };
                        Vector2f pos1 = (*last).getPosition(), pos2 = (*element).getPosition();
                        float radius = (*element).getRadius();
                        Vector2f orientation1 = { radius,0 };
                        Vector2f orientation2 = { -radius,0 };

                        if (counter % 3 == 0) {
                            orientation1 = { -radius ,radius };
                            orientation2 = { radius ,-radius };
                            arrowSize = { 5 ,5 };
                        }
                        float dist = distanceBetweenTwoPoints(pos1, pos2);
                        /// de terminat
                       if( dist < 230.0 )
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

                last = nullptr;
                counter = 0;

                for (auto element : copyNodesStack)
                {
                    //window->draw(*element);
                    if (counter == 0) {
                        (*element).setBorder(3, Color::Black);
                        (*element).setTextOnTop("top");
                    }
                    else
                    {
                        (*element).setBorder(0, Color::Black);
                        (*element).setTextOnTop("");
                    }

                    if (last != nullptr) {
                        Vector2f width = { 1,1 };
                        Vector2f arrowSize = { 8,8 };
                        Vector2f pos1 = (*last).getPosition(), pos2 = (*element).getPosition();
                        float radius = (*element).getRadius();
                        Vector2f orientation1 = { 0,radius };
                        Vector2f orientation2 = { 0,-radius };

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
                }

                last = nullptr;
                counter = 0;
                
                for (auto element : copyNodesQueue)
                {
                    //window->draw(*element);
                    if (counter == 0) {
                       (*element).setBorder(3, Color::Black);
                       (*element).setTextOnTop("front");
                    }
                    else if (counter == copyNodesQueue.size() - 1) {
                        (*element).setBorder(3, Color::Black);
                        (*element).setTextOnTop("tail");
                    }
                    else
                    {
                       (*element).setBorder(0, Color::Black);
                        (*element).setTextOnTop("");
                    }

                    if (last != nullptr) {
                        Vector2f width = { 1,1 };
                        Vector2f arrowSize = { 8,8 };
                        Vector2f pos1 = (*last).getPosition(), pos2 = (*element).getPosition();
                        float radius = (*element).getRadius();
                        Vector2f orientation1 = { 0,radius };
                        Vector2f orientation2 = { 0,-radius };

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
                }


                /// aici desenez nodurile
                for (auto element : copyNodesSLL)
                {
                    window->draw(*element);
                }
                for (auto element : copyNodesDLL)
                {
                    window->draw(*element);
                }
                for (auto element : copyNodesStack)
                {
                    window->draw(*element);
                }
                for (auto element : copyNodesQueue)
                {
                    window->draw(*element);
                }

                vector < Element* > copyElements;
                copyElements = Elements;

                /// aici desenez butoanele
                for (auto element : copyElements)
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
            m.unlock();
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
    ListSLL sll;
    ListDLL dll;
    ListStack stack;
    ListQueue queue;
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
    void pushNode(string val, int position, bool save = true)
    {
        if (save == true)
        {

            if (ds_type == "sll")   sll.addNode(val, position);
            if (ds_type == "dll")   dll.addNode(val, position);
        }
        int len = (*Vector).size();
        if (position > len) position = len;
        if (position == 0) pushFront(val, save = false);
        else if (position == len) pushBack(val, save = false);
        else
        {
            (*Vector).front()->setTextBelow("Node pred = head");

            for (int i = 0; i < position - 1; i++)
            {
                (*Vector)[i]->setColor(Color::Cyan);
                (*Vector)[i]->setTextBelow("for (cnt = 0; cnt < position - 1 ;cnt++)");
                sleep(speed * float(0.5));
                (*Vector)[i]->setTextBelow("// cnt = " + to_string(i));
                sleep(speed * float(0.5));
                (*Vector)[i]->setTextBelow("pred = pred.next");
                sleep(speed * float(0.5));
                (*Vector)[i]->setColor(Color::Red);
                (*Vector)[i]->setTextBelow("");
            }
            (*Vector)[position - 1]->setColor(Color::Cyan);
            (*Vector)[position - 1]->setTextBelow("Node after = pred.next");
            sleep(speed);
            (*Vector)[position - 1]->setTextBelow("");

            Node* nod = new Node(nodeSpawn_position, 25, val, "node" + ds_type + val, ds_type);
            (*Vector).insert((*Vector).begin() + position, nod);
            nod->setTextBelow("Node node = new Node");
            sleep(speed * 2.f);
            nod->setTextBelow("node.next = after");
            sleep(speed);
            (*Vector)[position - 1]->setTextBelow("pred.next = node");
            nod->setTextBelow("");
            sleep(speed);
            (*Vector)[position - 1]->setTextBelow("");
            (*Vector)[position - 1]->setColor(Color::Red);
            update();
        }
    }
    void pushFromFile(string val) {
        int counter = (*Vector).size();
        Node* nod = new Node(nodeSpawn_position, 25, val, "node" + ds_type + val, ds_type);
        (*Vector).insert((*Vector).begin() + counter, nod);
        sleep(speed);
        update();
    }
    void pushBack(string val, bool save = true)
    {
        if (save == true)
        {

            if (ds_type == "sll")   sll.addNode(val, sll.length);
            if (ds_type == "dll")   dll.addNode(val, dll.length);
            if (ds_type == "s")     stack.addNode(val, stack.length);/* stack.push(val);*/
            if (ds_type == "q")     queue.addNode(val, queue.length);
        }


        int counter = (*Vector).size();

        /// de pus else if pentru dll


        Node* nod = new Node(nodeSpawn_position, 25, val, "node" + ds_type + val, ds_type);
        (*Vector).insert((*Vector).begin() + counter, nod);

        // background color
        nod->setTextBelow("Node node = new Node");
        nod->setTextOnTopColor(Color(39.0 * 25, 39.0 * 25, 39.0 * 25));
        if (counter > 0)
        {
            sleep(speed);
            nod->setTextBelow("");
            (*Vector)[counter - 1]->setTextBelow("tail.next = node");
            sleep(speed);
            (*Vector)[counter - 1]->setTextBelow("");


        }
        else if (counter == 0)
        {
            sleep(speed);
            string temp = "head";
            if (ds_type == "q") temp = "front";
            nod->setTextBelow(temp + " = node");
            sleep(speed);
        }
        nod->setTextBelow("tail = node");
        sleep(speed);
        nod->setTextOnTopColor(Color::Black);
        sleep(speed);
        nod->setTextBelow("");
        update();


    }
    void pushFront(string val, bool save = true)
    {
        if (save == true)
        {

            if (ds_type == "sll")   sll.addNode(val, 0);
            if (ds_type == "dll")   dll.addNode(val, 0);
            if (ds_type == "s")     stack.addNode(val, 0);
            if (ds_type == "q")     queue.addNode(val, 0);
        }
        Node* nod = new Node(nodeSpawn_position, 25, val, "node" + ds_type + val, ds_type);
        (*Vector).insert((*Vector).begin(), nod);

        // background color
        nod->setTextOnTopColor(Color(39.0 * 25, 39.0 * 25, 39.0 * 25));

        string temp = "head";
        int size = (*Vector).size();
        if (ds_type == "s") temp = "top";

        nod->setTextBelow("Node node = new Node");
        sleep(speed);
        if (size > 1)
        {
            nod->setTextBelow("node.next = " + temp);
            sleep(speed);

        }
        nod->setTextBelow(temp + " = node");
        sleep(speed);
        if (size == 1)
        {
            nod->setTextBelow("tail = node");
            sleep(speed);
        }
        nod->setTextOnTopColor(Color::Black);
        nod->setTextBelow("");
        update();
    }

    void popBack(bool save = true)
    {
        int size = (*Vector).size();
        if (size > 0)
        {

            if (save == true)
            {

                if (ds_type == "sll")   sll.deleteNode(sll.length);
                if (ds_type == "dll")   dll.deleteNode(dll.length);
                if (ds_type == "s")     stack.deleteNode(stack.length);
                if (ds_type == "q")     queue.deleteNode(queue.length);
            }

            if (ds_type == "sll")
            {
                deleteNode((*Vector).size() - 1, save = false);
            }
            else if (ds_type == "dll")
            {
                (*Vector).back()->setColor(Color::Green);
                (*Vector).back()->setTextBelow("Node del = tail");
                sleep(speed);
                (*Vector).back()->setTextBelow("tail = tail.pred, delete del");
                sleep(speed);
                (*Vector).pop_back();
                update();
                (*Vector).back()->setTextBelow("tail.next = null");
                sleep(speed);
                (*Vector).back()->setTextBelow("");
            }
            else
            {


            }
        }
    }

    void popFront(bool save = true)
    {
        int size = (*Vector).size();
        if (size > 0)
        {

            if (save == true)
            {

                if (ds_type == "sll")   sll.deleteNode(0);
                if (ds_type == "dll")   dll.deleteNode(0);
                if (ds_type == "s")     stack.deleteNode(0);
                if (ds_type == "q")     queue.deleteNode(0);
            }
            string temp = "head";
            if (ds_type == "q") temp = "front";
            else if (ds_type == "s") temp = "top";
            if (size == 1)
            {
                (*Vector).front()->setColor(Color::Green);
                (*Vector).front()->setTextBelow(temp);
                sleep(speed);
                (*Vector).front()->setTextBelow("delete");
                (*Vector).front()->setTextBelowColor(Color::Red);
                sleep(speed);
                (*Vector).erase((*Vector).begin());
            }
            else
            {
                (*Vector).front()->setTextBelow("Node del = " + temp);
                sleep(speed);
                (*Vector).front()->setTextBelow(temp + " = " + temp + ".next , delete del");
                (*Vector).front()->setColor(Color::Green);
                (*Vector).front()->setTextOnTopColor(Color(39.0 * 25, 39.0 * 25, 39.0 * 25));
                sleep(speed);
                (*Vector).erase((*Vector).begin());

            }
            update();
        }
    }

    void deleteNode(int counter , bool save = true)
    {
        int size = (*Vector).size();
        if (size > 0 && counter >= 0 && counter < size)
        {
            if (save == true)
            {

                if (ds_type == "sll")   sll.deleteNode(counter);
                if (ds_type == "dll")   dll.deleteNode(counter);
            }

            if (counter > size) counter = size - 1;

            if (counter == 0) popFront(save=false);
            else
            {

                (*Vector)[0]->setTextBelow("Node pred = head");
                (*Vector)[0]->setColor(Color::Cyan);
                sleep(speed);

                for(int i=0 ; i < counter-1 ; i++)
                {
                    (*Vector)[i]->setColor(Color::Cyan);
                    (*Vector)[i]->setTextBelow("for (k = 0; k < counter-1; k++)");
                    sleep(speed);
                    (*Vector)[i]->setTextBelow("pred = pred.next");
                    sleep(speed);
                    (*Vector)[i]->setTextBelow("");
                    (*Vector)[i]->setColor(Color::Red);
                    (*Vector)[i]->setTextBelowColor(Color::Black);
                }
                (*Vector)[counter - 1]->setColor(Color::Cyan);
                (*Vector)[counter - 1]->setTextBelow("//pred");
                sleep(speed);
                (*Vector)[counter]->setColor(Color::Green);
                (*Vector)[counter - 1]->setTextBelow("");
                (*Vector)[counter]->setTextBelow("Node del = pred.next");
                sleep(speed);
                (*Vector)[counter]->setTextBelow("");
                if (counter == size - 1)
                {
                    (*Vector)[counter - 1]->setTextBelow("pred.next = null");
                    sleep(speed);
                    (*Vector)[counter - 1]->setTextBelow("delete del , tail = pred");
                    sleep(speed);
                }
                else
                {

                    (*Vector)[counter]->setTextBelow("Node after = del.next");
                    sleep(speed);
                    (*Vector)[counter]->setTextBelow("");
                    (*Vector)[counter - 1]->setTextBelow("pred.next = after");
                    sleep(speed * 0.5f);
                    (*Vector)[counter - 1]->setTextBelow("");
                    (*Vector)[counter]->setTextBelow("delete del");
                    sleep(speed);
                }
                (*Vector)[counter - 1]->setColor(Color::Red);
                (*Vector)[counter - 1]->setTextBelow("");
                (*Vector).erase((*Vector).begin()+counter);
                update();
            }
        }
    }

    void deleteNode(Node* nod)
    {
        //Nodes.erase(nod);
        sleep(speed);
        update();
    }

    void erase()
    {
        if (ds_type == "sll")   sll.emptyList();
        if (ds_type == "dll")   dll.emptyList();
        if (ds_type == "s")     stack.emptyList();
        if (ds_type == "q")     queue.emptyList();
        for (auto node : (*Vector))
        {
            requestForAnimation.push({ node,{-100,1500} });
        }
        // nu stiu de ce nu merge *
        sleep(speed);
        (*Vector).clear();

    }
    
    void saveNodes()
    {
        if (ds_type == "sll")   sll.printList("sll");
        if (ds_type == "dll")   dll.printList("dll");
        if (ds_type == "s")     stack.printList("s");
        if (ds_type == "q")     queue.printList("q");
    }
    
    void loadNodes()
    {
        if (ds_type == "sll")
        {
            sll.readFromFile("sll");
            node* itr = sll.first;
            while (itr != NULL) {
                pushFromFile(itr->data);
                itr = itr->next;
            }
        }
        if (ds_type == "dll")
        {
            dll.readFromFile("dll");
            node* itr = dll.first;
            while (itr != NULL) {
                pushFromFile(itr->data);
                itr = itr->next;
            }
        }
        if (ds_type == "s") {
            stack.readFromFile("s");
            node* itr = stack.first;
            while (itr != NULL) {
                pushFromFile(itr->data);
                itr = itr->next;
            }
        }
        if (ds_type == "q") {
            queue.readFromFile("q");
            node* itr = queue.first;
            while (itr != NULL) {
                pushFromFile(itr->data);
                itr = itr->next;
            }
        }
    }

    void speedDown()
    {
        if(speed < seconds(4))
            speed = speed + seconds(0.25);
    }
    void speedUp()
    {
        if(speed > seconds(0.5))
            speed = speed - seconds(0.25);
    }
};

DataStructureVisualizer SLL({ 100,320 }, 3, &NodesSLL);
DataStructureVisualizer DLL({ 450,320 }, 3, &NodesDLL, "dll");
DataStructureVisualizer S({ 850,320 }, 1, &NodesStack, "s");
DataStructureVisualizer Q({ 1100,320 }, 1, &NodesQueue, "q");

void switchLanguage(string lang) {
    if (lang == "ro") lang = "en";
    else if (lang == "en") lang = "ro";
    if(lang=="ro" || lang=="en")
    { 
        for (auto& buton : ButonDictionar)
        {
            buton.second->setText(Language[lang][buton.first]);
        }
    }
}

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
                if (optionForDS == "SLL")   SLL.erase();
                if (optionForDS == "DLL")   DLL.erase();
                if (optionForDS == "S")     S.erase();
                if (optionForDS == "Q")     Q.erase();
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

                    if (isPositiveNumber(pos) && val != Language[limba]["ti_addNodeData"]) {
                        SLL.pushNode(val, stoi(pos));
                    }
                    else if (val != Language[limba]["ti_addNodeData"])
                    {
                        SLL.pushBack(val);

                    }
                    else if (isPositiveNumber(pos))
                    {
                        SLL.pushNode(to_string(NodesSLL.size()),stoi(pos));
                    }
                    /*
                    else
                    {
                        SLL.pushBack(to_string(NodesSLL.size()));
                    }*/
                }
                //if (optionForDS == "SLL") SLL.pushNode(to_string(NodesSLL.size()), NodesSLL.size());
                else  if (optionForDS == "DLL") /*DLL.pushNode(to_string(NodesDLL.size()),NodesDLL.size());*/
                {
                    if (isPositiveNumber(pos) && val != Language[limba]["ti_addNodeData"]) {
                        DLL.pushNode(val, stoi(pos));
                    }
                    else if (val != Language[limba]["ti_addNodeData"])
                    {
                        DLL.pushBack(val);

                    }
                    else if (isPositiveNumber(pos))
                    {
                        DLL.pushNode(to_string(NodesDLL.size()), stoi(pos));
                    }
                    /*
                    else
                    {
                        DLL.pushBack(to_string(NodesDLL.size()));
                    }*/
                }
                ButonDictionar["ti_addNodeData"]->handleTextInput("");
                ButonDictionar["ti_addNodePos"]->handleTextInput("");
                ButonDictionar["ti_addNodeData"]->setText(Language[limba]["ti_addNodeData"]);
                ButonDictionar["ti_addNodePos"]->setText(Language[limba]["ti_addNodePos"]);

            }
        }
        else if (id == "pushNodeBtn")
        {
            if (event == "click")
            {
           // if (optionForDS == "S") S.pushNode(to_string(NodesStack.size()));
           // else  if (optionForDS == "Q") Q.pushNode(to_string(NodesQueue.size()));
                string val;
                val = ButonDictionar["ti_addNodeData"]->getText();
                if (optionForDS == "Q")
                {
                    if (val != Language[limba]["ti_addNodeData"])
                    {
                        Q.pushBack(val);
                    }
                    else
                        Q.pushBack(to_string(NodesQueue.size()));
                }
                if (optionForDS == "S")
                {
                    if (val != Language[limba]["ti_addNodeData"])
                    {
                        S.pushFront(val);
                    }
                    else
                        S.pushFront(to_string(NodesStack.size())); 
                }
                ButonDictionar["ti_addNodeData"]->handleTextInput("");
                ButonDictionar["ti_addNodePos"]->handleTextInput("");
                ButonDictionar["ti_addNodeData"]->setText(Language[limba]["ti_addNodeData"]);
                ButonDictionar["ti_addNodePos"]->setText(Language[limba]["ti_addNodePos"]);
            }
        }
        else if (id == "pushFrontBtn")
        {
            if (event == "click")
            {
                string val;
                val = ButonDictionar["ti_addNodeData"]->getText();
                if (optionForDS == "SLL")
                {
                    if (val != Language[limba]["ti_addNodeData"]) SLL.pushFront(val);
                    else
                        SLL.pushFront(to_string(NodesSLL.size()));
                }
                //if (optionForDS == "SLL") SLL.pushNode(to_string(NodesSLL.size()), NodesSLL.size());
                else  if (optionForDS == "DLL") /*DLL.pushNode(to_string(NodesDLL.size()),NodesDLL.size());*/
                {
                    if (val != Language[limba]["ti_addNodeData"]) DLL.pushFront(val);
                    else
                        DLL.pushFront(to_string(NodesDLL.size()));
                }
                ButonDictionar["ti_addNodeData"]->handleTextInput("");
                ButonDictionar["ti_addNodePos"]->handleTextInput("");
                ButonDictionar["ti_addNodeData"]->setText(Language[limba]["ti_addNodeData"]);
                ButonDictionar["ti_addNodePos"]->setText(Language[limba]["ti_addNodePos"]);
            }
        }
        else if (id == "pushBackBtn")
        {
            if (event == "click")
            {
                string val;
                val = ButonDictionar["ti_addNodeData"]->getText();
                if (optionForDS == "SLL")
                {
                    if (val != Language[limba]["ti_addNodeData"]) SLL.pushBack(val);
                    else
                        SLL.pushBack(to_string(NodesSLL.size()));
                }
                //if (optionForDS == "SLL") SLL.pushNode(to_string(NodesSLL.size()), NodesSLL.size());
                else  if (optionForDS == "DLL") /*DLL.pushNode(to_string(NodesDLL.size()),NodesDLL.size());*/
                {
                    if (val != Language[limba]["ti_addNodeData"]) DLL.pushBack(val);
                    else
                        DLL.pushBack(to_string(NodesDLL.size()));
                }
                ButonDictionar["ti_addNodeData"]->handleTextInput("");
                ButonDictionar["ti_addNodePos"]->handleTextInput("");
                ButonDictionar["ti_addNodeData"]->setText(Language[limba]["ti_addNodeData"]);
                ButonDictionar["ti_addNodePos"]->setText(Language[limba]["ti_addNodePos"]);
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
                    if (isPositiveNumber(pos) && !NodesSLL.empty()) {
                        SLL.deleteNode(stoi(pos));
                    }
                    /*else
                    {
                        SLL.popBack();
                    }*/
                }
                else if (optionForDS == "DLL") 
                {
                    if (isPositiveNumber(pos) && !NodesSLL.empty()) {
                        DLL.deleteNode(stoi(pos));
                    }
                    /*else
                    {
                        DLL.popBack();
                    }*/
                }
                ButonDictionar["ti_addNodeData"]->handleTextInput("");
                ButonDictionar["ti_addNodePos"]->handleTextInput("");
                ButonDictionar["ti_addNodeData"]->setText(Language[limba]["ti_addNodeData"]);
                ButonDictionar["ti_addNodePos"]->setText(Language[limba]["ti_addNodePos"]);
            }
        }
        else if (id == "popNodeBtn")
        {
            if (event == "click")
            {
                if (optionForDS == "Q") Q.popFront();

                if (optionForDS == "S") S.popFront();
            }
            ButonDictionar["ti_addNodeData"]->handleTextInput("");
            ButonDictionar["ti_addNodePos"]->handleTextInput("");
            ButonDictionar["ti_addNodeData"]->setText(Language[limba]["ti_addNodeData"]);
            ButonDictionar["ti_addNodePos"]->setText(Language[limba]["ti_addNodePos"]);
        }
        else if (id == "popFrontBtn")
        {
            if (event == "click")
            {
                if (optionForDS == "SLL")   SLL.popFront();

                if (optionForDS == "DLL")   DLL.popFront();
            }
            ButonDictionar["ti_addNodeData"]->handleTextInput("");
            ButonDictionar["ti_addNodePos"]->handleTextInput("");
            ButonDictionar["ti_addNodeData"]->setText(Language[limba]["ti_addNodeData"]);
            ButonDictionar["ti_addNodePos"]->setText(Language[limba]["ti_addNodePos"]);
        }
        else if (id == "popBackBtn")
        {
            if (event == "click")
            {
                if(optionForDS == "SLL")   SLL.popBack();

                if (optionForDS == "DLL")   DLL.popBack();
            }
            ButonDictionar["ti_addNodeData"]->handleTextInput("");
            ButonDictionar["ti_addNodePos"]->handleTextInput("");
            ButonDictionar["ti_addNodeData"]->setText(Language[limba]["ti_addNodeData"]);
            ButonDictionar["ti_addNodePos"]->setText(Language[limba]["ti_addNodePos"]);
        }
        else if (id == "sllBtn")
        {
            if (event == "click")
            {
                optionForDS = "SLL";
                ButonDictionar["newListBtn"]->makeVisible();
                //ButonDictionar["clearListBtn"]->makeVisible();
                
                ButonDictionar["addNodeBtn"]->makeVisible();
                ButonDictionar["pushNodeBtn"]->makeInvisible();
                ButonDictionar["pushFrontBtn"]->makeVisible();
                ButonDictionar["pushBackBtn"]->makeVisible();

                ButonDictionar["delNodeBtn"]->makeVisible();
                ButonDictionar["popNodeBtn"]->makeInvisible();
                ButonDictionar["popFrontBtn"]->makeVisible();
                ButonDictionar["popBackBtn"]->makeVisible();

                ButonDictionar["ti_addNodePos"]->makeVisible();
                ButonDictionar["ti_addNodeData"]->makeVisible();

                ButonDictionar["slowUp"]->makeVisible();
                ButonDictionar["speedUp"]->makeVisible();

                ButonDictionar["sllBtn"]->setColor(Color::Green);
                ButonDictionar["dllBtn"]->setColor(Color::Blue);
                ButonDictionar["stackBtn"]->setColor(Color::Blue);
                ButonDictionar["queueBtn"]->setColor(Color::Blue);
            }
        }
        else if (id == "dllBtn")
        {
            if (event == "click")
            {
                optionForDS = "DLL";
                ButonDictionar["newListBtn"]->makeVisible();
                //ButonDictionar["clearListBtn"]->makeVisible();

                ButonDictionar["addNodeBtn"]->makeVisible();
                ButonDictionar["pushNodeBtn"]->makeInvisible();
                ButonDictionar["pushFrontBtn"]->makeVisible();
                ButonDictionar["pushBackBtn"]->makeVisible();

                ButonDictionar["delNodeBtn"]->makeVisible();
                ButonDictionar["popNodeBtn"]->makeInvisible();
                ButonDictionar["popFrontBtn"]->makeVisible();
                ButonDictionar["popBackBtn"]->makeVisible();

                ButonDictionar["ti_addNodePos"]->makeVisible();
                ButonDictionar["ti_addNodeData"]->makeVisible();

                ButonDictionar["slowUp"]->makeVisible();
                ButonDictionar["speedUp"]->makeVisible();

                ButonDictionar["sllBtn"]->setColor(Color::Blue);
                ButonDictionar["dllBtn"]->setColor(Color::Green);
                ButonDictionar["stackBtn"]->setColor(Color::Blue);
                ButonDictionar["queueBtn"]->setColor(Color::Blue);
            }
        }
        else if (id == "stackBtn")
        {
            if (event == "click")
            {
                optionForDS = "S";
                ButonDictionar["newListBtn"]->makeVisible();
                //ButonDictionar["clearListBtn"]->makeVisible();

                ButonDictionar["addNodeBtn"]->makeInvisible();
                ButonDictionar["pushNodeBtn"]->makeVisible();
                ButonDictionar["pushFrontBtn"]->makeInvisible();
                ButonDictionar["pushBackBtn"]->makeInvisible();

                ButonDictionar["delNodeBtn"]->makeInvisible();
                ButonDictionar["popNodeBtn"]->makeVisible();
                ButonDictionar["popFrontBtn"]->makeInvisible();
                ButonDictionar["popBackBtn"]->makeInvisible();

                ButonDictionar["ti_addNodePos"]->makeInvisible();
                ButonDictionar["ti_addNodeData"]->makeVisible();

                ButonDictionar["slowUp"]->makeVisible();
                ButonDictionar["speedUp"]->makeVisible();

                ButonDictionar["sllBtn"]->setColor(Color::Blue);
                ButonDictionar["dllBtn"]->setColor(Color::Blue);
                ButonDictionar["stackBtn"]->setColor(Color::Green);
                ButonDictionar["queueBtn"]->setColor(Color::Blue);
            }
        }
        else if (id == "queueBtn")
        {
            if (event == "click")
            {
                optionForDS = "Q";
                ButonDictionar["newListBtn"]->makeVisible();
                //ButonDictionar["clearListBtn"]->makeVisible();

                ButonDictionar["addNodeBtn"]->makeInvisible();
                ButonDictionar["pushNodeBtn"]->makeVisible();
                ButonDictionar["pushFrontBtn"]->makeInvisible();
                ButonDictionar["pushBackBtn"]->makeInvisible();

                ButonDictionar["delNodeBtn"]->makeInvisible();
                ButonDictionar["popNodeBtn"]->makeVisible();
                ButonDictionar["popFrontBtn"]->makeInvisible();
                ButonDictionar["popBackBtn"]->makeInvisible();

                ButonDictionar["ti_addNodePos"]->makeInvisible();
                ButonDictionar["ti_addNodeData"]->makeVisible();

                ButonDictionar["slowUp"]->makeVisible();
                ButonDictionar["speedUp"]->makeVisible();

                ButonDictionar["sllBtn"]->setColor(Color::Blue);
                ButonDictionar["dllBtn"]->setColor(Color::Blue);
                ButonDictionar["stackBtn"]->setColor(Color::Blue);
                ButonDictionar["queueBtn"]->setColor(Color::Green);
            }
        }
        else if (id == "saveListsBtn")
        {
            if (event == "click")
            {
                if (optionForDS == "SLL")   SLL.saveNodes();
                if (optionForDS == "DLL")   DLL.saveNodes();
                if (optionForDS == "S")     S.saveNodes();
                if (optionForDS == "Q")     Q.saveNodes();
            }
        }
        else if (id == "loadListsBtn")
        {
            if (event == "click")
            {
                if (optionForDS == "SLL")   SLL.loadNodes();
                if (optionForDS == "DLL")   DLL.loadNodes();
                if (optionForDS == "S")     S.loadNodes();
                if (optionForDS == "Q")     Q.loadNodes();
            }
        }
        else if (id == "languageBtn")
        {
            if (event == "click")
            {
                switchLanguage(ButonDictionar["languageBtn"]->getText());
            }
        }
        else if (id == "speedUp") {
            SLL.speedUp();
            DLL.speedUp();
            S.speedUp();
            Q.speedUp();
        }
        else if (id == "slowUp") {
            SLL.speedDown();
            DLL.speedDown();
            S.speedDown();
            Q.speedDown();
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
    words();

    /// IN LOC SA CREEZ VARIABILE MAI BINE LE PUN IN DICTIONAR DIRECT
    ButonDictionar["sllBtn"] =new Buton({50,0}, {300,50}, Language[limba]["sllBtn"], "sllBtn");
    ButonDictionar["dllBtn"] = new Buton({ 400,0 }, { 300,50 }, Language[limba]["dllBtn"], "dllBtn");
    ButonDictionar["stackBtn"] = new Buton({ 750,0 }, { 200,50 }, Language[limba]["stackBtn"], "stackBtn");
    ButonDictionar["queueBtn"] = new Buton({ 1000,0 }, { 200,50 }, Language[limba]["queueBtn"], "queueBtn");

    ButonDictionar["newListBtn"] = new Buton({ 1200,200 }, { 290,50 }, Language[limba]["newListBtn"], "newListBtn");
    //ButonDictionar["clearListBtn"] = new Buton({ 1200,700 }, { 290,50 }, "Clear List", "clearListBtn");

    ButonDictionar["addNodeBtn"] = new Buton({ 1200,400 }, { 140,50 }, Language[limba]["addNodeBtn"], "addNodeBtn");
    ButonDictionar["pushNodeBtn"] = new Buton({ 1200,400 }, { 140,50 }, Language[limba]["pushNodeBtn"], "pushNodeBtn");
    ButonDictionar["pushFrontBtn"] = new Buton({ 1200,460 }, { 140,50 }, Language[limba]["pushFrontBtn"], "pushFrontBtn");
    ButonDictionar["pushBackBtn"] = new Buton({ 1200,520 }, { 140,50 }, Language[limba]["pushBackBtn"], "pushBackBtn");
   
    ButonDictionar["delNodeBtn"] = new Buton({ 1350,400 }, { 140,50 }, Language[limba]["delNodeBtn"], "delNodeBtn");
    ButonDictionar["popNodeBtn"] = new Buton({ 1350,400 }, { 140,50 }, Language[limba]["popNodeBtn"], "popNodeBtn");
    ButonDictionar["popFrontBtn"] = new Buton({ 1350,460 }, { 140,50 }, Language[limba]["popFrontBtn"], "popFrontBtn");
    ButonDictionar["popBackBtn"] = new Buton({ 1350,520 }, { 140,50 }, Language[limba]["popBackBtn"], "popBackBtn");
    
    
    
    ButonDictionar["saveLists"] = new Buton({ 1220,0 }, { 120,50 }, Language[limba]["saveLists"], "saveListsBtn");
    ButonDictionar["loadLists"] = new Buton({ 1350,0 }, { 120,50 }, Language[limba]["loadLists"], "loadListsBtn");

    ButonDictionar["ti_addNodePos"] = new Buton({ 1200,300 }, { 140,50 }, Language[limba]["ti_addNodePos"], "ti_addNodePos");
    ButonDictionar["ti_addNodeData"] = new Buton({ 1350,300 }, { 140,50 }, Language[limba]["ti_addNodeData"], "ti_addNodeData");

    ButonDictionar["languageBtn"] = new Buton({ 1400,720 }, { 60,40 }, limba, "languageBtn");

    ButonDictionar["slowUp"] = new Buton({ 50,720 }, { 120,40 }, Language[limba]["slowUp"], "slowUp");
    ButonDictionar["speedUp"] = new Buton({ 190, 720 }, { 120,40 } , Language[limba]["speedUp"] , "speedUp");



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
                /// onHover la noduri dezactivat temporar

                /*
                for (auto element : Nodes)
                {
                    if (element->isInRange(mousePosition))
                    {
                        target = element;
                        target->onHover();
                    }
                }
                */
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