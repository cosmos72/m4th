/**
 * Copyright (C) 2020 Massimiliano Ghilardi
 *
 * This file is part of m4th.
 *
 * m4th is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * m4th is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with m4th.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifdef __cplusplus
extern "C" {
#endif

enum {
    H(m4hash_no_entry_) = (H(m4u)) - 1,
    H(m4hash_no_next_) = (H(m4u)) - 2,
};

typedef struct H(m4hash_entry_s_) {
    m4cell val;
    H(m4) key;
    H(m4u) next;
} H(m4hash_entry_);

typedef struct H(m4hash_map_s_) {
    H(m4u) size;
    H(m4u) lcap; /* capacity is 1<<lcap */
    H(m4hash_entry_) * vec;
} H(m4hash_map_);

H(m4hash_map_) * H(m4hash_map_new_)(H(m4u) capacity);
void H(m4hash_map_del_)(H(m4hash_map_) * map);

// find key in map. return NULL if not found
const H(m4hash_entry_) * H(m4hash_map_find_)(const H(m4hash_map_) * map, H(m4) key);

// insert key and val.
// key MUST NOT be already present. does not grow/rehash.
// returns NULL on failure (if map is too full)
const H(m4hash_entry_) * H(m4hash_map_insert_)(H(m4hash_map_) * map, H(m4) key, m4cell val);

#ifdef __cplusplus
}
#endif