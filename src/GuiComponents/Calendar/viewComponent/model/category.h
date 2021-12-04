#ifndef CATEGORY_H
#define CATEGORY_H

using namespace std;

class Category
{
private:
    unsigned int id;
    string name;
    string color;

public:
    Category (Category &category) {
        this->name = category.getName();
        this->color = category.getColor();
        this->id = category.getId();
    }

    Category(unsigned int id, const string &name, const string &color) {
        this->name = name;
        this->color = color;
        if (id == 0)
            this->id = static_cast<unsigned int> (hash<string>()(this->name + this->color));
        else
            this->id = id;
    }

    ~Category() {
    }

    string getName() { return name; }
    string getColor() { return color; }
    unsigned int getId() { return id; }

    bool equals(Category &c) {
        return (this->id == c.getId());
    }

    long hashcode() {
        return static_cast<long> (this->id);
    }
};

#endif // CATEGORY_H
