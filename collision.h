#include "structs.h"

void object_update(int i, int j, OBJ *loaded) {
	loaded[j].render = 0;
	loaded[i].size += 3;
	loaded[i].score += loaded[j].size * 10;
}

void collision(int i, int j, OBJ *loaded) {
	if (i != 1 && j != 1 && loaded[i].size > loaded[j].size
			&& loaded[i].render == 1 && loaded[j].render == 1
			&& abs(loaded[i].x - loaded[j].x) < loaded[i].size
			&& abs(loaded[i].y - loaded[j].y) < loaded[i].size && i != j) {
		object_update(i, j, loaded);
	}

}
