#pragma once
#include "flat_map.h"

template <class Id, class Product, class Creator>
class Factory {
public:
	Product * createProductById(const Id& id) {
		auto it = creators.find(id);
		if (creators.end() == it) {
			return nullptr;
		}
		
		return (*it).second();
	}
	bool registerProduct(const Id& id, const Creator& creator) {
		creators[id] = creator;
		return true;
	}

	static Factory * getInstance() {
		static Factory f;
		return &f;
	}
private:
	Factory() = default;
	Factory(const Factory&) = delete;
	Factory& operator=(const Factory&) = delete;
	flat_map<Id, Creator> creators;
};