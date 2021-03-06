#ifndef HERBIVORE_H
#define HERBIVORE_H

#include "Animal.h"

class Herbivore : public Animal
{
public:

	Herbivore(const bool isMale) noexcept;
	~Herbivore() noexcept;

	bool isPredator() const override;
};

#endif