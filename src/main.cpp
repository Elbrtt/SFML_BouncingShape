#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include <fstream>

using namespace std;

unsigned int WindowWidth = 800;
unsigned int WindowLength = 600;

class CustomShape
{
private:
	string name;
	unsigned int wWidth, wLength;

public:
	CustomShape(const string& name) : name(name){};

	virtual void doMove() = 0;

	virtual void render(sf::RenderWindow& window) = 0;

	string getName() const
	{
		return name;
	}

	virtual ~CustomShape() {};
};

class Rect : public CustomShape
{
private:
	sf::RectangleShape r;
	float sX, sY;		
	float direction = 1;
	MyEnum a = DEFAULT;
	MyEnum b = DEFAULT;
	sf::Text text;

public:

	Rect() = default;
	Rect(const string& name, float posX, float posY, float sX, float sY,
		unsigned int R, unsigned int G, unsigned int B,
		float sW, float sL, const sf::Text& text)
		: CustomShape(name), text(text)
	{
		this->sX = sX;
		this->sY = sY;
		r.setSize(sf::Vector2f(sW, sL));
		r.setFillColor(sf::Color(R, G, B));
		r.setPosition({ posX,posY });
	
	}

	void doMove() override
	{
   
		if (r.getPosition().x <= 0) sX = sX * -direction;
		else if (r.getPosition().x >= WindowWidth - r.getSize().x)sX = sX * -direction;
		if (r.getPosition().y <= 0) sY = sY * -direction;
		else if (r.getPosition().y >= WindowLength - r.getSize().y)sY = sY * -direction;

		r.move({ sX,sY });

		text.setPosition(
			{ r.getPosition().x + (r.getSize().x/8),
			 r.getPosition().y + (r.getSize().y/2 - text.getCharacterSize())}
		);

	}

	void render(sf::RenderWindow& window) override
	{
		window.draw(r);
		window.draw(text);
	}

	~Rect() {};

};

class Circ : public CustomShape
{
private: 
	sf::CircleShape c;
	float sX, sY;
	float direction = 1;
	MyEnum a = DEFAULT;
	MyEnum b = DEFAULT;
	sf::Text text;
public:
	Circ(const string& name, float posX, float posY, float sX, float sY,
		unsigned int R, unsigned int G, unsigned int B,
		float Radius, sf::Text& text)
		: CustomShape(name), sX(sX), sY(sY), text(text)
	{
		c.setRadius(Radius);
		c.setFillColor(sf::Color(R, G, B));
		c.setPosition({ posX,posY });
	}

	void doMove() override
	{

		if (c.getPosition().x <= 0) sX = sX * -direction;
		else if (c.getPosition().x >= WindowWidth - 2 * c.getRadius())sX = sX * -direction;
		if (c.getPosition().y <= 0) sY = sY * -direction;
		else if (c.getPosition().y >= WindowLength - 2 * c.getRadius())sY = sY * -direction;

		c.move({ sX,sY });

		text.setPosition(
			{ c.getPosition().x +(c.getRadius()/2),
			c.getPosition().y + (c.getRadius() - text.getCharacterSize())}
		);

	}

	void render(sf::RenderWindow& window) override
	{
		window.draw(c);
		window.draw(text);
	}


	~Circ() {};
};

class ShapeManager
{
private:
	vector<shared_ptr<CustomShape>> shapes;
	string name;
	string type;
	sf::Vector2f initPos;

	unsigned int wWidth;
	unsigned int wLength;

	float SX, SY;
	float direction = 1;

public:
	ShapeManager() = default;

	void addShape(const shared_ptr<CustomShape>& shape)
	{
		shapes.push_back(shape);
	}

	void moveShape()
	{
		for (auto& s : shapes)
		{
			s->doMove();
		}
	}

	void setWindowSize(unsigned int x, unsigned int y)
	{
		wWidth = x;
		wLength = y;
	}

	void renderShape(sf::RenderWindow& window)
	{
		for (auto& s : shapes)
		{
			s->render(window);
		}
	};


	~ShapeManager() {};
};

class fileManager
{
private:

	unsigned int wWidth = 800;
	unsigned int wLength = 600;

	string font = "backrooms.ttf";
	int fR = 0, fG= 0, fB = 0;
	unsigned int fontSize;
	sf::Font fnt;
	sf::Text text;

public:
	ShapeManager sm;

	//fileManager() = default;

	fileManager()
		: fnt("assets/backrooms.ttf"),
		text(fnt)
	{
		text.setString("Hello");
		text.setCharacterSize(24);
	}

	~fileManager() {};
	void loadFile()
	{
		string type;

		float sW;
		float sL;
		float radius;

		unsigned int R, G, B;

		string name;
		float posX, posY;
		float sX, sY;

		ifstream filein("assets/config.txt");

		if (!filein.good())
		{
			cout << "config not found!" << endl;
		}

		while (filein >> type)
		{
			if (type.compare("Window") == 0)
			{
				filein >> wWidth >> wLength; 
				WindowWidth = wWidth;
				WindowLength = wLength;
			}
			else if (type.compare("Font") == 0)
			{
				filein >> font >> fontSize >> fR >> fG >> fB;
				fnt.openFromFile(font);
			}
			else { //TODO SPEED
				text.setFont(fnt);
				text.setFillColor(sf::Color(fR, fG, fB));

				if (type.compare("Rectangle") == 0)
				{

					filein >> name >> posX >> posY >> sX >> sY >> R >> G >> B >> sW >> sL ;
					cout << "Rect";
					text.setString(name);
					auto rect = make_shared<Rect>(name,posX,posY,sX,sY,R,G,B,sW,sL,text);

					sm.addShape(rect);
				}
				if (type.compare("Circle") == 0)
				{
					filein >> name >> posX >> posY >> sX >> sY >> R >> G >> B >> radius;
					text.setString(name);
					auto circ = make_shared<Circ>(name, posX, posY, sX, sY, R, G, B, radius, text);

					sm.addShape(circ);
				}
			}
		}
	}
};


int main(int argc, char * argv[])
{	
	fileManager fm;
	vector<sf::RectangleShape> rv;
	for (float i = 0; i < WindowWidth/10; i++)
	{
		for (float j = 0; j < WindowLength/10; j++)
		{
			sf::RectangleShape r({ 10,10 });
			r.setPosition({ i * 10,j * 10 });
			r.setFillColor(sf::Color( i/j, (i + j)/2, i * j));
			rv.push_back(r);
		}
	}

	fm.loadFile();

	cout << WindowWidth << WindowLength;
	sf::RenderWindow window(sf::VideoMode({ WindowWidth,WindowLength }), "Bouncing Shapes");
	window.setFramerateLimit(60);
	std::cout << "SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "."  << SFML_VERSION_PATCH << std::endl;

	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 72; j++)
		{
			sf::RectangleShape r({ 10,10 });
			r.setFillColor(sf::Color(127 + i, 127 + j, i + j));
			window.draw(r);
		}
	}

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>()) window.close();
		}

		window.clear();


		for (auto& r : rv)
		{
			window.draw(r);
		}


		fm.sm.moveShape();
		fm.sm.renderShape(window);

		window.display(); 
	}
}