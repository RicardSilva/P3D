#pragma once
#include "Material.h"
#include <map>


	namespace engine {

		class MaterialManager {

		private:
			static MaterialManager *_instance;
			std::map<std::string, Material*> map;
			MaterialManager() {};
			MaterialManager(MaterialManager const&) {};
			MaterialManager& operator=(MaterialManager const&) {};


		public:

			void AddMaterial(const std::string& name, Material *material);
			Material *GetMaterial(const std::string& name);

			static MaterialManager *Instance();
			void Destroy();


		};
}