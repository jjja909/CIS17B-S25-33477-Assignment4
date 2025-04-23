#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>

class DuplicateItemException : public std::runtime_error {
public:
    DuplicateItemException(const std::string& msg) : std::runtime_error(msg) {}
};

class ItemNotFoundException : public std::runtime_error {
public:
    ItemNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};

class StoredItem {
private:
    std::string id;
    std::string description;
    std::string location;

public:
    StoredItem(std::string id, std::string desc, std::string loc)
        : id(id), description(desc), location(loc) {}

    std::string getId() const { return id; }
    std::string getDescription() const { return description; }
    std::string getLocation() const { return location; }
};

class StorageManager {
private:
    std::unordered_map<std::string, std::shared_ptr<StoredItem>> itemById;
    std::map<std::string, std::shared_ptr<StoredItem>> itemByDescription;

public:
    void addItem(const std::shared_ptr<StoredItem>& item) {
        // TODO: Add item to both maps, throw if ID already exists
        const auto& id = item->getId();
        if (itemById.count(id)) {
            throw DuplicateItemException("Item using id " + id + " already exists!");
        }
        itemById[id] = item;
        itemByDescription[item->getDescription()] = item;
    }

    std::shared_ptr<StoredItem> findById(const std::string& id) const {
        auto it = itemById.find(id);
        if (it == itemById.end()) {
            throw ItemNotFoundException("Item using id " + id + " not found.");
        }
        // TODO: Return item if found or throw ItemNotFoundException
        return it->second;
    }

    void removeItem(const std::string& id) {
        // TODO: Remove from both maps, throw if not found
        auto it = itemById.find(id);
        if (it == itemById.end()) {
            throw ItemNotFoundException("Item using id " + id + " not found.");
        }
        
        auto desc = it->second->getDescription();
        itemById.erase(it);
        
        auto it2 = itemByDescription.find(desc);
        if (it2 != itemByDescription.end() && it2->second->getId() == id) {
            itemByDescription.erase(it2);
        }
    }

    void listItemsByDescription() const {
        // TODO: Iterate over itemByDescription and print info
        for (const auto& pair : itemByDescription) {
            const auto& item = pair.second;
            std::cout << "- " << item->getDescription()
                    << ": " << item->getLocation() << "\n";
        }
    }
};

void testDuplicateAddition() {
    StorageManager mgr;
    auto item = std::make_shared<StoredItem>(
        "ITEM001", "LED Light", "Aisle 3, Shelf 1"
    );
    

    mgr.addItem(item);

  
    try {
        mgr.addItem(item);
    }
    catch (const DuplicateItemException& e) {
        std::cout << "[Test] caught duplicate addition: " 
                  << e.what() << "\n";
    }
} 

    

void testItemNotFound() {
    StorageManager mgr;
    try {
        mgr.findById("NOITEM");
    } catch (const ItemNotFoundException& e) {
        std::cout << "[Test] Caught item not found: " << e.what() << "\n";
    }
}

int main() {
    // TODO: Create StorageManager and test functionality
    StorageManager mgr;
    
    //sample items
    auto item1 = std::make_shared<StoredItem>("ITEM001", "LED Light", "Aisle 3, Shelf 1");
    auto item2 = std::make_shared<StoredItem>("ITEM002", "Fan Motor", "Aisle 2, Shelf 5");
    
    //add items
    std::cout << "Adding item: " << item1->getId() << " - " << item1->getDescription() << "\n";
    mgr.addItem(item1);
    std::cout << "Adding item: " << item2->getId() << " - " << item2->getDescription() << "\n";
    mgr.addItem(item2);
    
    //attempting dupes
    std::cout << "Attempting to add ITEM001 again...\n";
    try {
        mgr.addItem(item1);
    } catch (const DuplicateItemException& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    
    //retrieving existing
     std::cout << "Retrieving ITEM002...\n";
    try {
        auto found = mgr.findById("ITEM002");
        std::cout << "Found: " << found->getDescription()
                  << " at " << found->getLocation() << "\n";
    } catch (...) {}
     
     //removing nonexistents
     std::cout << "Removing ITEM003...\n";
    try {
        mgr.removeItem("ITEM003");
    } catch (const ItemNotFoundException& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
     
     std::cout << "Items in Description Order:\n";
     mgr.listItemsByDescription();
     
     testDuplicateAddition();
     testItemNotFound();
     
     return 0;
}

