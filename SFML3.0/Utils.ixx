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
		float SqrMagnitude(const sf::Vector2<T>& _v)
	{
		return (_v.x * _v.x) + (_v.y * _v.y);
	}
	/*
	* Heavy comput length because of squareRoot, give "true" length
	* Too slow for projection kinda stuff
	*/
	template<typename T> constexpr const
		float Magnitude(const sf::Vector2<T>& _v)
	{
		return std::sqrtf(SqrMagnitude(_v));
	}
	/*
	* Dot Product > 0 same dir
	* Dot Product < 0 opposite dir
	* Dot Product = 0, orthogonal -> 90Deg
	* Dot Product = 1, if normalized vector : ||u|| . ||v|| . cos(theta) = colinear
	* Give alignment
	*/
	template<typename T> constexpr
		float DotProduct(const sf::Vector2<T>& _u, const sf::Vector2<T>& _v)
	{
		return _u.x * _v.x + _u.y * _v.y;
	}

	/*
	* determinant inspired by matrices 2x2
	* = ||u|| . ||v|| . sin(theta)
	* det > 0, rotation counter clockwise
	* det < 0, rotation clockwise
	* det = 0, colinear
	* det = 1, normalized ||u|| . ||v|| . sin(theta) = +1||-1 = orthonal +90||-90
	* Give direction && signed perpendicularity || orthogonal height, read above cases
	*/
	template<typename T> constexpr
		float DetProduct(const sf::Vector2<T>& _u, const sf::Vector2<T>& _v)
	{
		return _u.x * _v.y - _u.y * _v.x;
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

	/*
	* Unsigned Angle Cos 0-PI
	*/
	template<typename T> constexpr
		float UnsignedAngle(sf::Vector2<T> _u, sf::Vector2<T> _v)
	{
		return std::acos(DotProduct(_u, _v) / Magnitude(_u) * Magnitude(_v));
	}

	template<typename T> constexpr
		float SignedAngle(sf::Vector2<T> _u, sf::Vector2<T> _v)
	{
		//Cos -> magnitude
		float dot = DotProduct(_u, _v);
		//Sin -> Rotation
		float det = DetProduct(_u, _v);
		//Y & X
		return std::atan2f(det, dot);
	}

	template<typename T> constexpr
		sf::Vector2<T> Normalize(const sf::Vector2<T>& _v)
	{
		float mag = Magnitude(_v);
		float denom = 1.0f / mag;
		return sf::Vector2<T>(_v.x * denom, _v.y * denom);
	}

	/*
	* Return scalar number representing len along the direction
	*/
	template<typename T>
	float ProjectPoint(const sf::Vector2<T>& _u, const sf::Vector2<T>& _dir)
	{
		float dot = DotProduct(_u, _dir);
		float sqMag = SqrMagnitude(_dir);
		return dot / sqMag;
	}

	/*
	* ProjectVec Vec on Vec with direction
	* (u.v/(|v|^2)) . v
	*/
	template<typename T> constexpr
		sf::Vector2<T> ProjectVec(const sf::Vector2<T>& _u, const sf::Vector2<T>& _dir)
	{
		float scalar = ProjectPoint(_u, _dir);
		return scalar * _dir;
	}
}