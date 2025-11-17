module;
#include "Common.h"
#include <numbers>
export module Utils;
export namespace Utils
{
	/*
	* Simple Length without need of squareRoot, give the "fake" length
	*/
	template<typename T> constexpr
		float SqrMagnitude(const sf::Vector2<T>& _vec) 
	{
		return (_vec.x * _vec.x) + (_vec.y * _vec.y);
	}
	/*
	* Heavy comput length because of squareRoot, give "true" length
	* Too slow for projection kinda stuff
	*/
	template<typename T> constexpr const
		float Magnitude(const sf::Vector2<T>& _vec)
	{
		return std::sqrtf(SqrMagnitude(_vec));
	}
	/*
	* Dot Product > 0 same dir
	* Dot Product < 0 opposite dir
	* Dot Product = 0, orthogonal -> 90Deg 
	* Dot Product = 1, if normalized vector : ||u|| . ||v|| . cos(theta) = colinear 
	* Give alignment
	*/
	template<typename T> constexpr
		float DotProduct(const sf::Vector2<T>& _vec1, const sf::Vector2<T>& _vec2)
	{
		return _vec1.x * _vec2.x + _vec1.y * _vec2.y;
	}

	/*
	* RAD  |   PI
	* ----------- = RAD * 180.0f / PI(3.14159....)
	* DEG  | 180.0f
	*/
	constexpr float RadToDeg(const float _val)
	{
		return _val * 180.0f / std::numbers::pi;
	}

	/*
	* RAD  |   PI
	* ----------- = DEG * PI(3.14159....) / 180.0f
	* DEG  | 180.0f
	*/
	constexpr float DegToRad(const float _val)
	{
		return _val * std::numbers::pi / 180.0f;
	}

	template<typename T> constexpr
		sf::Vector2<T> Normalize(const sf::Vector2<T>& _vec)
	{
		float mag = Magnitude(_vec);
		float denom = 1.0f / mag;
		return sf::Vector2<T>(_vec.x * denom, _vec.y * denom);
	}

	/*
	* Not truly a CrossProduct : determinant inspired by matrices 2x2
	* = ||u|| . ||v|| . sin(theta)
	* det > 0, rotation counter clockwise
	* det < 0, rotation clockwise
	* det = 0, colinear
	* det = 1, normalized ||u|| . ||v|| . sin(theta) = +1||-1 = orthonal +90||-90
	* Give direction && signed perpendicularity || orthogonal height, read above cases
	*/
	template<typename T> constexpr
		float CrossProduct(const sf::Vector2<T>& _vec1, const sf::Vector2<T>& _vec2)
	{
		return _vec1.x * _vec2.y - _vec1.y * _vec2.x;
	}

	/*
	* Project a point/Vector unto another Point/Vector
	* u.v/||v|| . v/||v|| = u.v/(||v||^2) . v
	*/
	template<typename T> constexpr
		sf::Vector2<T> Project(const sf::Vector2<T>& _vec1, const sf::Vector2<T>& _vec2)
	{
		float dot = DotProduct(_vec1, _vec2);
		float sqMag = SqrMagnitude(_vec2);
		return (dot / sqMag) * _vec2;
	}
}