/* tree.h */

#ifndef TREE_H
#define TREE_H

typedef struct TreeNode TreeNode;

typedef struct Child {
    TreeNode *node;
    struct Child *next;
} Child;

struct TreeNode {
    double volume;          // Volume d'eau entrant
    double leak_percent;    // % de fuite depuis le parent
    Child *children;        // Liste chaînée des enfants
};

TreeNode *tree_node_create(double volume, double leak);
void tree_node_add_child(TreeNode *parent, TreeNode *child);

#endif

////////////////

/* tree.c */

#include <stdlib.h>
#include "tree.h"

TreeNode *tree_node_create(double volume, double leak) {
    TreeNode *n = malloc(sizeof(TreeNode));
    if (!n) return NULL;

    n->volume = volume;
    n->leak_percent = leak;
    n->children = NULL;
    return n;
}

void tree_node_add_child(TreeNode *parent, TreeNode *child) {
    Child *c = malloc(sizeof(Child));
    c->node = child;
    c->next = parent->children;
    parent->children = c;
}

//////////////////////

/* avl.h */

#ifndef AVL_H
#define AVL_H

typedef struct AVLNode {
    char *key;                  // id acteur aval
    TreeNode *tree_node;        // pointeur vers arbre normal
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

AVLNode *avl_insert(AVLNode *root, const char *key, TreeNode *tree_node);
TreeNode *avl_find(AVLNode *root, const char *key);

#endif

//////////////////////

TreeNode *new_node;
if (is_source_to_factory) {
    new_node = tree_node_create(volume, leak_percent);
} else {
    new_node = tree_node_create(0.0, leak_percent);
}


//////////////////////


void process_line(
    AVLNode **avl,
    TreeNode *root,
    const char *id_parent,
    const char *id_child,
    double volume,
    double leak
) {
    // 1️⃣ Recherche du parent dans l’AVL
    TreeNode *parent_node = avl_find(*avl, id_parent);

    // 2️⃣ Création du nœud enfant
    TreeNode *child_node = tree_node_create(volume, leak);

    // 3️⃣ Placement dans l’arbre normal
    if (parent_node) {
        tree_node_add_child(parent_node, child_node);
    } else {
        // parent inconnu → on accroche au nœud racine fictif
        tree_node_add_child(root, child_node);
    }

    // 4️⃣ Ajout dans l’AVL (clé = acteur aval)
    *avl = avl_insert(*avl, id_child, child_node);
}

////////////////////////////

int is_source_to_factory(char **cols) {
    return strstr(cols[1], "Spring") != NULL;
}

////////////////////////////

if (is_source_to_factory(cols)) {
    // SOURCE → USINE
    if (strcmp(cols[2], usine_id) == 0) {
        process_line(
            &avl,
            root,
            cols[1],      // parent = source
            cols[2],      // enfant = usine
            atof(cols[3]),
            atof(cols[4])
        );
    }
} else {
    // AUTRES TRONÇONS
    if (strcmp(cols[0], usine_id) == 0) {
        process_line(
            &avl,
            root,
            cols[1],      // acteur amont
            cols[2],      // acteur aval
            0.0,
            atof(cols[4])
        );
    }
}

////////////////////////////////

TreeNode *root = tree_node_create(0.0, 0.0);

//////////////////////////////

double compute_losses(TreeNode *node, double incoming_volume);

///////////////////////////


