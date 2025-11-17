#include <iostream>
#include "Common.h"
#include <cstdint>
import Utils;
using namespace Utils;

constexpr const char16_t WINDOW_W = 1280;
constexpr const char16_t WINDOW_H = 768;
constexpr const char16_t CIRCLE_RAD = 360;
constexpr const char8_t TEXT_SIZE = 24;
constexpr const uint64_t CIRCLE_POINTS_COUNT = CIRCLE_RAD * static_cast<char8_t>(2);

enum class TextType : int
{
	Angle,
	Cosinus,
	Sinus
};
int main()
{
	sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_W, WINDOW_H)), "Triglolo", sf::Style::Default);
	sf::Vector2f mousePos{ 0.0f,0.0f };
	sf::Vector2f endPoint{ 0.0f,0.0f };

	sf::Font mainFont;
	if (!mainFont.openFromFile("Assets/Fonts/college.ttf")) {
		assert("Error loading font, check existence or path");
	}

	std::array<sf::Text, 3> texts = {
	(mainFont),
	(mainFont),
	(mainFont)
	};

	for (unsigned i = 0; i < texts.size(); i++)
	{
		texts[i].setCharacterSize(TEXT_SIZE);
		texts[i].setFillColor(sf::Color::White);
		texts[i].setPosition(sf::Vector2f(texts[i].getPosition().x, TEXT_SIZE * i));
	}

	//Circle setup
	sf::CircleShape circle(CIRCLE_RAD, CIRCLE_POINTS_COUNT);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(sf::Color::White);
	circle.setOutlineThickness(3.f);
	circle.setOrigin(sf::Vector2f(CIRCLE_RAD, CIRCLE_RAD));
	circle.setPosition(sf::Vector2f((WINDOW_W / 2.0f), (WINDOW_H / 2.0f)));

	//Line
	constexpr auto MakeLine = [&](const sf::Vector2f& _pos0, const sf::Vector2f& _pos1, const sf::Color& _color)
		-> sf::VertexArray
		{
			sf::VertexArray line(sf::PrimitiveType::Lines, 2);
			line[0].position = _pos0;
			line[1].position = _pos1;
			line[0].color = _color;
			line[1].color = _color;
			return line;
		};

	auto vectorLine = MakeLine(circle.getPosition(), sf::Vector2f(0.0f, 0.0f), sf::Color::Cyan);
	auto cosinusLine = MakeLine(
		sf::Vector2f(circle.getPosition().x - CIRCLE_RAD, circle.getPosition().y),
		sf::Vector2f(circle.getPosition().x + CIRCLE_RAD, circle.getPosition().y),
		sf::Color::Red);
	auto sinusLine = MakeLine(
		sf::Vector2f(circle.getPosition().x, circle.getPosition().y - CIRCLE_RAD),
		sf::Vector2f(circle.getPosition().x, circle.getPosition().y + CIRCLE_RAD),
		sf::Color::Green);

	auto lineX = MakeLine(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f), sf::Color::Red);
	auto lineY = MakeLine(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f), sf::Color::Green);
	auto tanLine = MakeLine(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f), sf::Color::Magenta);

	//Vectors ->
	constexpr auto CreateVector = [&](const sf::Vector2f& _start, const sf::Vector2f& _end)
		-> sf::Vector2f
		{
			sf::Vector2f vectorValue = _end - _start;
			return vectorValue;
		};

	sf::Vector2f baseLineXvec = CreateVector(lineX[1].position, lineX[0].position);
	sf::Vector2f baseLineYvec = CreateVector(lineY[1].position, lineY[0].position);
	sf::Vector2f userLineVec = CreateVector(vectorLine[1].position, vectorLine[0].position);
	sf::Vector2f cosLineVec = CreateVector(cosinusLine[1].position, cosinusLine[0].position);
	sf::Vector2f sinLineVec = CreateVector(sinusLine[1].position, sinusLine[0].position);

	//Other
	const sf::Vector2f vector1Norm = Normalize(cosinusLine[1].position - circle.getPosition());
	const float circleRadSqr = circle.getRadius() * circle.getRadius();
	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>() ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				window.close();
			}
		}
		mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

		//user Vector(Blue)
		const sf::Vector2f offset = mousePos - circle.getPosition();
		const float offsetSqrLen = SqrMagnitude(offset);
		if (offsetSqrLen > circleRadSqr)
		{
			sf::Vector2f direction = Normalize(offset);
			endPoint = circle.getPosition() + direction * circle.getRadius();
		}
		else
		{
			endPoint = mousePos;
		}
		vectorLine[1].position = endPoint;

		//Angle
		const sf::Vector2f vector2Norm = Normalize(offset);
		/*
		* SFML inverting y, for easing of use inverted back to mathematical coordinate thus -y
		*/
		const_cast<float&>(vector2Norm.y) = -vector2Norm.y;
		const float det = CrossProduct(vector1Norm, vector2Norm);
		const float dot = DotProduct(vector1Norm, vector2Norm);
		const float angleRad = std::atan2(det, dot);
		const float angleDeg = RadToDeg(angleRad);

		//CosAngle
		const float cosTheta = acosf(DotProduct(vector1Norm, vector2Norm) / (Magnitude(vector1Norm) * Magnitude(vector2Norm)));
		const float cosf = std::cosf(cosTheta);

		//SinAngle
		const float scalar = CrossProduct(vector1Norm, vector2Norm);
		const float sinTheta = asinf(scalar / (Magnitude(vector1Norm) * Magnitude(vector2Norm)));
		const float sinf = std::sinf(sinTheta);

		//Render
		const sf::Vector2f tip = vectorLine[1].position;

		constexpr auto ProjectVecOnVec = [&](const sf::Vector2f& _startPoint, const sf::Vector2f& _baseVec, const sf::Vector2f& _dirVec) 
			-> sf::Vector2f
			{
			sf::Vector2f baseVec = _baseVec;
			sf::Vector2f dir = Normalize(_dirVec);
			sf::Vector2f offset = _startPoint - baseVec;
			sf::Vector2f proj = Project(offset, dir);
			return baseVec + proj;
			};

		//Point projection on cosline
		const sf::Vector2f projPointCos = ProjectVecOnVec(tip, cosinusLine[0].position, cosLineVec);
		lineX[0].position = tip;
		lineX[1].position = projPointCos;

		//Point project on sinLine
		const sf::Vector2f projPointSin = ProjectVecOnVec(tip, sinusLine[0].position, sinLineVec);
		lineY[0].position = tip;
		lineY[1].position = projPointSin;

		//Tan position 
		const sf::Vector2f CirlePos = circle.getPosition();
		/*
		* SFML inverted Y coordinate thus the - before (yb - ya)
		*/
		const sf::Vector2f dir = Normalize(sf::Vector2f(tip.x - CirlePos.x, -(tip.y - CirlePos.y)));
		const sf::Vector2f startPoint = tip + dir * 2000.f;
		const sf::Vector2f endPoint = tip - dir * 2000.f;
		tanLine[0].position = startPoint;
		tanLine[1].position = endPoint;

		//Misc Render
		texts[static_cast<int>(TextType::Angle)].setString("Angle: " + std::format("{:.2f}", angleDeg));
		texts[static_cast<int>(TextType::Cosinus)].setString("Cos: " + std::format("{:.8f}", cosf));
		texts[static_cast<int>(TextType::Sinus)].setString("Sin: " + std::format("{:.8f}", sinf));

		//Render Final
		window.clear(sf::Color::Black);
		window.draw(circle);
		window.draw(cosinusLine);
		window.draw(sinusLine);
		window.draw(lineX);
		window.draw(lineY);
		window.draw(tanLine);
		window.draw(vectorLine);

		for (const auto& text : texts)
		{
			window.draw(text);
		}
		window.display();
	}
	return EXIT_SUCCESS;
}