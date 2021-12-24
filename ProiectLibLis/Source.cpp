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

#define FRAME_INTERVAL 15
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

    void move(Vector2f direction)
    {
        //circle.move(direction);
        //cout<<"APELAT"<<endl;
        setPosition(position + direction);
    }

    static Node getNewNode()
    {
        //return new Node();
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

// TODO: IN LOC DE NODE TRB TRANSFORMABLE , inainte cand puneam transformable coada nu prelua adresa nodului corect
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

    Texture texture;
    if (!texture.loadFromFile("./Images/arrow.png"))
        return;
    Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(0.2, 0.2);

    RectangleShape rectangle;
    ConvexShape triangle;
    CircleShape circle1, circle2, circle3;
    //Vector2f pos1(500, 450);
    //Vector2f pos2(550, 550);
    Vector2f pos1(632, 344);
    Vector2f pos2(376, 123);
    //cout << distanceBetweenTwoPoints(pos1, pos2);
    Vector2f pos3(pos2.x, pos1.y);
    float angle = asin(distanceBetweenTwoPoints(pos2, pos3) / distanceBetweenTwoPoints(pos1, pos2)) * (180.0 / 3.141592653589793238463);
    //cout << "\n" << angle;
    rectangle.setSize({distanceBetweenTwoPoints(pos1, pos2), 2 });
    //rectangle.setSize({ 107.703, 5 });
    if (pos1.x < pos2.x) {
        if (pos1.y < pos2.y) {
            rectangle.setRotation(angle);
            sprite.setRotation(angle);
        }
        if (pos1.y > pos2.y) {
            rectangle.setRotation(360-angle);
            sprite.setRotation(360 - angle);
        }
    }
    if (pos1.x > pos2.x) {
        if (pos1.y < pos2.y) {
            rectangle.setRotation(180-angle);
            sprite.setRotation(180 - angle);
        }
        if (pos1.y > pos2.y) {
            rectangle.setRotation(180+angle);
            sprite.setRotation(180 + angle);
        }
    }

        
    //rectangle.setRotation(40.f);
    rectangle.setPosition(pos1);
    rectangle.setFillColor(Color::Red);
    /*triangle.setFillColor(Color::Red);

    triangle.setPointCount(3);
    triangle.setPoint(0, pos1);
    triangle.setPoint(1, pos2);
    triangle.setPoint(2, pos3);*/

    sprite.setOrigin(pos2);
    sprite.setPosition(pos2);

    circle1.setRadius(10.f);
    circle1.setFillColor(Color::Blue);
    circle1.setPosition(pos1);
    circle1.setOrigin(10.f, 10.f);
    circle2.setRadius(10.f);
    circle2.setFillColor(Color::Cyan);
    circle2.setPosition(pos2);
    circle2.setOrigin(10.f, 10.f);
    circle3.setRadius(10.f);
    circle3.setFillColor(Color::Green);
    circle3.setPosition(pos3);
    circle3.setOrigin(10.f, 10.f);

    while (window->isOpen())
    {
        if (myClock.getElapsedTime() - timeSnapshot > time_interval)
        {
            window->clear(Color::White);
            /// AICI AR TREBUI DRAWABLE
            for (auto element : Elements)
            {
                window->draw(*element);
            }
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
                window->draw(*element);
            }
            for (auto element : NodesQueue)
            {
                window->draw(*element);
            }

            window->draw(rectangle);
            window->draw(circle1);
            window->draw(circle2);
            window->draw(circle3);
            window->draw(sprite);
            //window->draw(triangle);

            window->display();
            timeSnapshot = myClock.getElapsedTime();
            //myClock.restart();
        }
    }

}

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

float distanceBetweenTwoPoints(Vector2f point1, Vector2f point2) {
    return sqrtf(powf((point2.x - point1.x), 2) + powf((point2.y - point1.y), 2));
}

void DrawLine(Vector2f pos1, Vector2f pos2) {
    RectangleShape rectangle;
    Vector2f pos3(pos1.x, pos2.y);
    float angle = asin(distanceBetweenTwoPoints(pos2, pos3));
    rectangle.setSize(Vector2f(distanceBetweenTwoPoints(pos1, pos2), 10));
    rectangle.setRotation(angle);
    rectangle.setPosition(pos1);
    rectangle.setFillColor(Color::Red);
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