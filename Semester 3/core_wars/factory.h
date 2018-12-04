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
		
		/*Creator creator = (*it).second;
		Product * u = creator();
		return u;*/
		return (*it).second();
	}
	bool registerProduct(const Id& id, const Creator& creator) {
		// TODO register_twice
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