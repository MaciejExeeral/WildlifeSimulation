#ifndef ANIMAL_H
#define ANIMAL_H

#include <iostream>

class Tile;

class Animal
{

public:

	friend class Tile;

protected:

	int m_id{};
	bool m_isMale{};
	bool m_hasMoved{};

	Tile* m_currentTile{};

	inline static int animalID{};
	inline static int totalAmountOfPredators{};
	inline static int totalAmountOfHerbivores{};

public:

	Animal(const bool isMale) noexcept;
	~Animal() noexcept = default;

	virtual bool isPredator() const = 0;
	void setCurrentTile(Tile* tile);
	void setHasMoved(const bool input);

	bool isMale() const;
	int getId() const;
	bool hasMoved() const;


	static int getTotalAmountOfPredators()
	{
		return totalAmountOfPredators;
	}

	static int getTotalAmountOfHerbivores()
	{
		return totalAmountOfHerbivores;
	}

	friend std::ostream& operator<<(std::ostream& out, const Animal& animal);
};

#endif