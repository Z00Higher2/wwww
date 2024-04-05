#include "Pantry.hpp"
#include <iostream>

Pantry::Pantry() 
{
   
}

Pantry::Pantry(const std::string& filename) : LinkedList <Ingredient*>()
{
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) 
    {
        std::cerr << "Error: Unable to open file." << std::endl;
        exit(1);
    }
    std::string line;
    std::string junk;
    std::getline(filename, junk);
    
    while(getline(filename,line))
    {
        std::string name, description;
        int quantity, price;
        std::vector<Ingredient*> recipe;
        std::istringstream ss(line);

        
        std::getline(ss, name, ',');
        std::getline(ss, description, ',');
        std::getline(ss, quantity, ',');
        std::getline(ss, price, ',');
        std::getline(ss, recipe, ',');
    }
    inputFile.close();
    }

    // inputFile.close(); <---- was outside the function Pantry

int Pantry::getPosOf(const std::string& name) const 
{
    int pos = 0;
    Node<Ingredient*>* current_node = getHeadNode();

    while (current_node != nullptr) 
    {
        if (current_node->getItem()->name == name) 
        {
            return pos;
        }
        current_node = current_node->getNext();
        pos++;
    }
    return -1;
}
/**
    @param: A const string reference to a ingredient name
    @return: True if the ingredient information is already in the Pantry
*/
bool Pantry::contains(const std::string& name) const 
{
    if(getPosOf(name) != -1)
    {
        return true;
    }
    return false;
}
/**
    @param:  A pointer to an Ingredient object
    @post:  Inserts the given ingredient pointer into the Pantry, unless an ingredient of the same name is already in the pantry. 
            Each of its Ingredients in its recipe are also added to the Pantry IF not already in the list.
    @return: True if the ingredient was added successfully, false otherwise.
*/
bool Pantry::addIngredient(Ingredient* ingredient) 
{
    Node<Ingredient*> eIngredients = getPointerTo(getPosOf(ingredient->name_));
    if (!contains(ingredient->name_) && !eIngredients) 
    {
        insert(ingredient);
        return true;
    }
    return false;
}
/**
    @param: A const string reference representing a ingredient name
    @param: A const string reference representing ingredient description
    @param: A const int reference representing the ingredient's quantity
    @param: A const int reference representing the ingredient's price
    @param: A const reference to a vector holding Ingredient pointers representing the ingredient's recipe
    @post:   Creates a new Ingredient object and inserts a pointer to it into the Pantry. 
            Each of its Ingredients in its recipe are also added to the Pantry IF not already in the list.
    @return: True if the ingredient was added successfully
*/
bool Pantry::addIngredient(const std::string& name, const std::string& description, int quantity, int price, const std::vector<Ingredient*>& recipe)
{
    Ingredient* newIngredient = new Ingredient(name, description, quantity, price, recipe);
    return addIngredient(newIngredient);
}

/**
    @param:  A Ingredient pointer
    @return: A boolean indicating if all the given ingredient can be created (all of the ingredients in its recipe can be created, or if you have enough of each ingredient in its recipe to create it)
*/
bool Pantry::canCreate(Ingredient* ingredient) 
{
   if(ingredient -> recipe.empty())
   {
    return true; 
   }
   for(int i = 0; i < ingredient->recipe.size(); i++)
   {
    Ingredient* recIngredient = ingredient->recipe[i];
    if(!recIngredient || !canCreate(recIngredient) || !contains(recIngredient -> name_))
    {
        return false;
    }
   }
   return true;
}
/**
    @param: A Ingredient pointer
    @post: Prints the ingredient name, quantity, and description.
    The output should be of the form:
    [Ingredient Name]: [Quantity]\n
    [Ingredient Description]\n
    Price: [Price]\n
    Recipe:\n
    [Ingredient0] [Ingredient1]\n


    If the ingredient has no recipe, print "Recipe:\nNONE\n\n" after the price.
*/
void Pantry::printIngredient(Ingredient* ingredient) const 
{
    std::cout << ingredient->name << ": " << std::endl;
    std::cout << ingredient->quantity << std::endl;
    std::cout << ingredient->description << std::endl;
    std::cout << "Price: " << ingredient->price << std::endl;
    std::cout << "Recipe:\n";
    if (ingredient->recipe.empty()) 
    {
        std::cout << "NONE\n\n";
    } 
    else 
    {
        for (int i = 0;  : ingredient->recipe.size(); i++) 
        {
            std::cout << ingredient->recipe[i]->name << " ";
        }
        std::cout << "\n\n";
    }
}
/**
    @param: A const string reference to a ingredient name
    @post:  Prints a list of ingredients that must be created before the given ingredient can be created (missing ingredients for its recipe, where you have 0 of the needed ingredient).
            If the ingredient is already in the pantry, print "In the pantry([quantity])\n"
            If there are no instances of the ingredient, if it cannot be crafted because of insufficient ingredients, print "[Ingredient Name](0)\nMISSING INGREDIENTS"
            If it can be crafted, recursively print the ingredients that need to be used (if there are instances of them) or created (if there are no instances of them) in the order that the ingredients appear in the recipe, joined by "<-".
            If the ingredient has no recipe, print "UNCRAFTABLE\n" at the end of the function.

            Below are some of the expected forms. "Scenario: [scenario] is not part of the output. It is just to help you understand the expected output.":

            Scenario: The Ingredient does not exist in the list
            Query: [Ingredient Name]
            No such ingredient

            Scenario: The Ingredient exists in the list, and there are >0 instances of it
            Query: [Ingredient Name]
            In the pantry ([Quantity])

            Scenario: The Ingredient exists in the list, and there are 0 instances of it, and it is craftable by using an ingredient that is already in the pantry
            Query: [Ingredient Name0]
            [Ingredient Name0](C)
            [Ingredient Name1](1)

            Scenario: The Ingredient exists in the list, and there are 0 instances of it, and it is craftable by using an ingredient that has to be crafted
            Query: [Ingredient Name0]
            [Ingredient Name0](C)
            [Ingredient Name1](C) <- [Ingredient Name2](3)
            
            Scenario: The Ingredient exists in the list, and there are 0 instances of it, and there are multiple ingredients that have to be crafted (each row represents a different ingredient inå the original recipe)
            Query: [Ingredient Name0]
            [Ingredient Name0](C)
            [Ingredient Name1](C) <- [Ingredient Name2](3)
            [Ingredient Name3](C) <- [Ingredient Name4](C) <- [Ingredient Name5] (3)

            Scenario: The Ingredient exists in the list, and there are 0 instances of it, and it is not craftable (it has no recipe)
            Query: [Ingredient Name0]
            UNCRAFTABLE

            Scenario: The Ingredient exists in the list, and there are 0 instances of it, and it has a recipe, but you do not have enough of the ingredients to craft it
            Query: [Ingredient Name0]
            [Ingredient Name0](0)
            MISSING INGREDIENTS
    
    HINT: Use canCreate() to determine if the ingredient can be created.
*/
void Pantry::ingredientQuery(const std::string& name_) const 
{
     Ingredient* ingredient = new Ingredient(name_);
    {
        std::cout << "No such ingredient\n";
    }
    if (ingredient->quantity_ > 0) 
    {
        std::cout << "In the pantry (" << ingredient->quantity_ << ")\n";
    }
    if (!canCreate(ingredient)) 
    {
        std::cout << ingredient->name_ << "(0)\nMISSING INGREDIENTS\n";

    }
    if(ingredient->recipe.empty())
    {
        std::cout << ingredient->name_ << "(0)\nUNCRAFTABLE\n";
        
    }
}

/**
    @return: An integer sum of the price of all the ingredients currently in the list.
    Note: This should only include price values from ingredients that you have 1 or more of. Do not consider ingredients that you have 0 of, even if you have the ingredients to make them.
*/

int Pantry::calculatePantryValue() const 
{
    int totalValue = 0;
    Node<Ingredient*>* current_node = head;
    
}
// Im done setting up the code here 
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Pantry::pantryList(const std::string& filter) const 
{
  for(int i = 0; i < ingredients.size(); i++){
    // With default filter "NONE": Print out every ingredient in the list.
    if(ingredients[i] == "CONTAINS"){
        // Get the name of the ingredient and the price of the ingredient
    } else if (ingredients[i] == "MISSING"){
        // Only print out the ingredients with 0 instances in the list.
    } else if (ingredients[i] == "CRAFTABLE"){
       // Only print out the ingredients where you have all the ingredients to craft them.
    } else if (ingredients[i] != "CONTAINS" && ingredients[i] != "MISSING" && ingredients[i] != "CRAFTABLE"){
        //  If an invalid filter is passed, print "INVALID FILTER\n"
    }

    //  Printing ingredients should be of the form:

    //     [Ingredient name]: [Quantity]
    //     [Ingredient description]\n
    //     Price: [price]\n
    //     Recipe:\n
    //     [Ingredient0] [Ingredient1]\n\n

    //     If the ingredient has no recipe, print "Recipe:\nNONE\n\n" after the price
  }
}