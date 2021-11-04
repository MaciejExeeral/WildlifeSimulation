#include <iostream>
#include <algorithm>

#include "Handler.h"
#include "Animal.h"
#include "Herbivore.h"
#include "Predator.h"
#include "functions.h"

Handler::Handler(int worldWidth, int worldHeight, int maxTurns)
	:m_worldWidth{ worldWidth },
	m_worldHeight{ worldHeight },
	m_maxAmountOfTurns{ maxTurns }
{
	m_tiles.reserve(m_worldHeight* m_worldWidth);

	for (int y{}; y <= m_worldHeight; ++y)
	{
		for (int x{}; x <= m_worldWidth; ++x)
		{
			m_tiles.push_back(new Tile{ x, y });
		}
	}
}

void Handler::populateWorld(int herbivores, int predators)
{
	int animalsToAdd{ herbivores + predators };
	for (int counter{}; counter < animalsToAdd; ++counter)
	{
		if (herbivores > 0)
		{
			addAnimal(new Herbivore{ static_cast<bool>(functions::generateRandomNumber(0, 1)) });
			--herbivores;
		}
		else
		{
			addAnimal(new Predator{ static_cast<bool>(functions::generateRandomNumber(0, 1)) });
		}
	}
}

void Handler::addAnimal(Animal* animal)
{
	if (animal)
	{
		int randomTile{ functions::generateRandomNumber(0, m_tiles.size() - 1) };

		m_tiles[randomTile]->addAnimalToTile(animal);
	}
	else
	{
		functions::printNullptrError();
	}
}

void Handler::printAllAnimalsInfo() const
{
	for (auto& currentTile : m_tiles)
	{
		if (currentTile->areAnimalsOnTile())
		{
			currentTile->printAnimalsInfo();
		}
	}
}

Tile* Handler::getTileByCoordinates(int x, int y) const
{
	auto found{ std::find_if(m_tiles.begin(), m_tiles.end(), [x, y](const Tile* currentTile)
		{
			return (currentTile->getXCoordinate() == x) && (currentTile->getYCoordinate() == y);
		}
	) };

	if (found != m_tiles.end())
	{
		return *found;
	}

	return nullptr;
}

void Handler::startSimulation()
{
	std::cout << "\t_--===INITIAL ENVIRONMENT===--_\n";
	printAllAnimalsInfo();
	std::cout << "\t_--===INITIAL ENVIRONMENT===--_\n";

	std::cin.get();

	for (int currentTurn{ 1 }; currentTurn <= m_maxAmountOfTurns; ++currentTurn)
	{
		std::cout << "\tTURN: " << currentTurn << '\n';

		std::cout << "\tALL HERBIVORES: " << Herbivore::getTotalAmountOfHerbivores() << '\n';
		std::cout << "\tALL PREDATORS: " << Predator::getTotalAmountOfPredators() << '\n';

		std::cout << "\n\t---Resetting all animals---\n";
		animalStateReset();
		std::cout << "\n\t---Movement Phase---\n";
		movementPhase();
		std::cout << "\n\t---Hunting Phase---\n";
		huntingPhase();
		std::cout << "\n\t---Breeding Phase---\n";
		breedingPhase();

		if (Herbivore::getTotalAmountOfHerbivores() == 0)
		{
			std::cout << "\n\t------<THE END>------\n\n";
			std::cout << "\tAll herbivores have died out!\n\n";
			std::cout << "\t------<THE END>------\n";
			return;
		}
		else if (Predator::getTotalAmountOfPredators() == 0)
		{
			std::cout << "\n\t------<THE END>------\n\n";
			std::cout << "\tAll predators have died out!\n\n";
			std::cout << "\t------<THE END>------\n";
			return;
		}

		std::cout << "\n\t_--===TURN " << currentTurn << " END RESULTS===--_\n";
		printAllAnimalsInfo();

		std::cout << "\n\tALL HERBIVORES: " << Herbivore::getTotalAmountOfHerbivores() << '\n';
		std::cout << "\n\tALL PREDATORS: " << Predator::getTotalAmountOfPredators() << '\n';

		std::cout << "\t_--===TURN " << currentTurn << " END RESULTS===--_\n";

		std::cin.get();

	}

	std::cout << "\n\t------<THE END>------\n\n";
	std::cout << "\tMax amount of turns reached!\n\n";
	std::cout << "\t------<THE END>------\n";
}

void Handler::animalStateReset()
{
	for (auto& currentTile : m_tiles)
	{
		if (currentTile->areAnimalsOnTile())
		{
			currentTile->resetAnimalsState();
		}
	}
}

void Handler::movementPhase()
{
	for (auto& currentTile : m_tiles)
	{
		if (currentTile->areAnimalsOnTile())
		{
			moveAnimalsFromTileToAdjacent(currentTile);
		}
	}
}

void Handler::huntingPhase()
{
	for (auto& currentTile : m_tiles)
	{
		if (currentTile->getAmountOfPredatorsOnTile() > 0)
		{
			currentTile->findPredatorAndEatHerbivore();
		}
	}
}

//TODO
void Handler::breedingPhase()
{
	int amountOfBreeds{};

	for (auto& currentTile : m_tiles)
	{
		if (currentTile->getAmountOfAnimalsOnTile() > 0)
		{
			amountOfBreeds = currentTile->getAmountOfHerbivoreBreeds();

			while (amountOfBreeds != 0)
			{
				std::cout << "Herbivores breeding at " << *currentTile << '\n';

				addAnimal(new Herbivore{ static_cast<bool>(functions::generateRandomNumber(0, 1)) });
				--amountOfBreeds;
			}

			amountOfBreeds = currentTile->getAmountOfPredatorBreeds();

			while (amountOfBreeds != 0)
			{
				std::cout << "Predators breeding at " << *currentTile << '\n';

				addAnimal(new Predator{ static_cast<bool>(functions::generateRandomNumber(0, 1)) });
				--amountOfBreeds;
			}
		}
	}
}

void Handler::moveAnimalsFromTileToAdjacent(Tile* tile)
{
	if (tile)
	{
		int movementPossibility{};
		int movementDirectionality{};

		int currentX{};
		int currentY{};

		Animal* currentAnimal{ nullptr };
		Tile* targetTile{ nullptr };

		for (int counter{}; counter < tile->getAmountOfAnimalsOnTile(); ++counter)
		{
			currentAnimal = tile->getAnimalOnIndex(counter);

			if (currentAnimal)
			{
				if (!currentAnimal->hasMoved())
				{

					movementPossibility = functions::generateRandomNumber(0, 3);
					movementDirectionality = functions::generateRandomNumber(0, 1);

					currentX = tile->getXCoordinate();
					currentY = tile->getYCoordinate();


					switch (movementPossibility)
					{
					case 0:
						if (movementDirectionality == 0)
						{
							currentX += 1;
						}
						else
						{
							currentX -= 1;
						}

						break;

					case 1:
						if (movementDirectionality == 0)
						{
							currentY += 1;
						}
						else
						{
							currentY -= 1;
						}

						break;

					case 2:
						if (movementDirectionality == 0)
						{
							currentX += 1;
							currentY += 1;
						}
						else
						{
							currentX -= 1;
							currentY -= 1;
						}

						break;

					case 3:
						if (movementDirectionality == 0)
						{
							currentX -= 1;
							currentY += 1;
						}
						else
						{
							currentX += 1;
							currentY -= 1;
						}

						break;

					default:
						std::cout << "Handler::moveAnimalToRandomAdjacentTile ERROR: Unspecified case!\n";
						return;
					}

					//if animal is on map's edge and wants to move to non-existing tile,
					//wrap his location around the map
					if (currentX > m_worldWidth)
					{
						currentX = 0;
					}
					else if (currentX < 0)
					{
						currentX = m_worldWidth;
					}

					if (currentY > m_worldHeight)
					{
						currentY = 0;
					}
					else if (currentY < 0)
					{
						currentY = m_worldHeight;
					}

					targetTile = getTileByCoordinates(currentX, currentY);

					currentAnimal->setCurrentTile(targetTile);
					targetTile->addAnimalToTile(currentAnimal);

					tile->removeAnimal(currentAnimal);

					std::cout << *currentAnimal << " moved from " << *tile
						<< " to " << *targetTile << ".\n\n";

					currentAnimal->setHasMoved(true);

					--counter;
				}
			}
			else
			{
				functions::printNullptrError();
			}
		}
	}
	else
	{
		functions::printNullptrError();
	}
}