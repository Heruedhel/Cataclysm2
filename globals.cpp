#include "field.h"
#include "globals.h"
#include "datapool.h"
#include "files.h"
#include "game.h"
#include "map.h"

int TESTING_MODE;

Game                      GAME;
Data_pool<Terrain>        TERRAIN;
Data_pool<World_terrain>  WORLD_TERRAIN;
Data_pool<Item_type>      ITEM_TYPES;
Data_pool<Item_group>     ITEM_GROUPS;
Data_pool<Monster_genus>  MONSTER_GENERA;
Data_pool<Monster_type>   MONSTER_TYPES;
Data_pool<Biome>          BIOMES;
Data_pool<Field_type>     FIELDS;
Data_pool<Furniture_type> FURNITURE_TYPES;
Data_pool<Profession>     PROFESSIONS;
Submap_pool               SUBMAP_POOL;
Mapgen_spec_pool          MAPGEN_SPECS;
Keybinding_pool           KEYBINDINGS;

void load_global_data()
{
  TERRAIN.load_from         (DATA_DIR + "/terrain.dat"        );
  WORLD_TERRAIN.load_from   (DATA_DIR + "/world_terrain.dat"  );
  FIELDS.load_from          (DATA_DIR + "/fields.dat"         );
  ITEM_TYPES.load_from      (DATA_DIR + "/items.dat"          );
  ITEM_GROUPS.load_from     (DATA_DIR + "/item_groups.dat"    );
  FURNITURE_TYPES.load_from (DATA_DIR + "/furniture.dat"      );
  MONSTER_GENERA.load_from  (DATA_DIR + "/monster_genera.dat" );
  MONSTER_TYPES.load_from   (DATA_DIR + "/monsters.dat"       );
  BIOMES.load_from          (DATA_DIR + "/biomes.dat"         );
  PROFESSIONS.load_from     (DATA_DIR + "/professions.dat"    );
  KEYBINDINGS.load_from     (DATA_DIR + "/keybindings.txt"    );

  load_mapgen_specs();
}

void load_mapgen_specs()
{
  std::vector<std::string> mapgen_files = files_in("data/mapgen", "map");
  for (int i = 0; i < mapgen_files.size(); i++) {
    std::string filename = "data/mapgen/" + mapgen_files[i];
    if (!filename.empty() && filename.find("/.") == std::string::npos) {
      MAPGEN_SPECS.load_from(filename);
    }
  }

// Now confirm we have a mapgen_spec for every WORLD_TERRAIN
  for (std::list<World_terrain*>::iterator it = WORLD_TERRAIN.instances.begin();
       it != WORLD_TERRAIN.instances.end();
       it++) {
    if (MAPGEN_SPECS.lookup_terrain_ptr( (*it) ).empty()) {
      debugmsg("No mapgen specs for %s!", (*it)->name.c_str());
    }
  }
}

bool prep_directories()
{
  std::vector<std::string> requirements;
// Add any required directories here.
  requirements.push_back( SAVE_DIR );
  requirements.push_back( SAVE_DIR + "/worlds" );

  for (int i = 0; i < requirements.size(); i++) {
    if (!directory_exists( requirements[i] )) {
      if (!create_directory( requirements[i] )) {
        debugmsg("Couldn't create %s.", requirements[i].c_str());
        return false;
      }
    }
  }
  return true;
}
