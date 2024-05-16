#include "bec.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void perev(int *one, int *two){
        int pr;
        pr = *one;
        *one = *two;
        *two = pr;
}

void increase(int size, int *one, int *two, int *tree, int *four, int *oneinfo, int *twoinfo, int *treeinfo, int *fourinfo){
        int pr;
        if (size==2){
                if (*one>*two){
                        perev(one, two);
                        perev(oneinfo, twoinfo);
                }
        }
        if (size==3){
                if (*one>*tree){
                        perev(one, tree);
                        perev(oneinfo, treeinfo);
                }
                if (*two>*tree){
                        perev(two, tree);
                        perev(twoinfo, treeinfo);
                }
        }
        if (size==4){
                if (*one>*four){
                        perev(one, four);
                        perev(oneinfo, fourinfo);
                }
                if (*two>*four){
                        perev(two, four);
                        perev(twoinfo, fourinfo);
                }
                if (*tree>*four){
                        perev(tree, four);
                        perev(treeinfo, fourinfo);
                }
        }
}

int isLeaf(Tree *tree){
        return (tree->sub[0]==NULL && tree->sub[1]==NULL && tree->sub[2]==NULL && tree->sub[3]==NULL);
}

Tree *scroll(Tree *tree, int key){
        if (tree->key[0]==key || tree->key[1]==key || tree->key[2]==key)
                return tree;
        return NULL;
}

Tree *find(Tree *tree, int key, Tree **pred){
        if (!tree) return NULL;
        if (scroll(tree, key)) *pred = tree;
        if (!tree || (isLeaf(tree) && !scroll(tree, key))) return NULL;
        if (scroll(tree, key) && isLeaf(tree)) return tree;
        else if (key <= tree->key[0]) return find(tree->sub[0], key, pred);
        else if (((tree->size>1) && (key<=tree->key[1])) || (tree->size==1)) return find(tree->sub[1], key, pred);
        else if (((tree->size>2) && (key<=tree->key[2])) || (tree->size==2)) return find(tree->sub[2], key, pred);
        else return find(tree->sub[3], key, pred);
}

Tree *findReal(Tree *ptr, Tree *pred){
        if (ptr) return ptr;
        else if (pred) return pred;
        else return NULL;
}

Tree *new(int key, int info, Tree *sub0, Tree *sub1, Tree *sub2, Tree *sub3, Tree *sub4, Tree *par){
        Tree *new = malloc(1*sizeof(Tree));
        new->key[0] = key;
        new->key[1] = 0;
        new->key[2] = 0;
        new->key[3] = 0;

        new->sub[0] = sub0;
        new->sub[1] = sub1;
        new->sub[2] = sub2;
        new->sub[3] = sub3;
        new->sub[4] = sub4;

        new->info = malloc(1*sizeof(Info));
        for (int i = 0; i<4; i++){
                new->info->info[i] = 0;
        }

        new->info->info[0] = info;
        new->parent = par;

        new->size = 1;
        return new;
}

void insert(Tree **tree, int key, int info, Tree *par){
        (*tree)->parent = par;
        (*tree)->key[((*tree)->size)] = key;
        (*tree)->info->info[(*tree)->size] = info;
        ((*tree)->size)+=1;
        increase((*tree)->size, &((*tree)->key[0]), &((*tree)->key[1]), &((*tree)->key[2]), &((*tree)->key[3]), &((*tree)->info->info[0]), &((*tree)->info->info[1]), &((*tree)->info->info[2]), &((*tree)->info->info[3]));
        return;
}

void add(Tree **TREE, Tree **tree, int key, int info, Tree *par){
        if (*tree==NULL){
                *tree = new(key, info, NULL, NULL, NULL, NULL, NULL, NULL);
                return;
        }
        Tree *pred = NULL, *p = NULL;
        Tree *tr = findReal(find((*tree), key, &pred), pred);
        if (tr){
                return;
        }
        if (isLeaf(*tree)) insert(tree, key, info, par);
        else if (key <= (*tree)->key[0]) add(TREE, &((*tree)->sub[0]), key, info, *tree);
        else if ((((*tree)->size>1) && (key<=(*tree)->key[1])) || ((*tree)->size==1)) add(TREE, &((*tree)->sub[1]), key, info, *tree);
        else if ((((*tree)->size>2) && (key<=(*tree)->key[2])) || ((*tree)->size==2)) add(TREE, &((*tree)->sub[2]), key, info, *tree);
        else add(TREE, &((*tree)->sub[3]), key, info, *tree);

        split(TREE, *tree);
        return;
}

void sub(Tree **sub0, Tree **sub2, Tree *ptr){
        *sub0 = new(ptr->key[0], ptr->info->info[0], ptr->sub[0], ptr->sub[1], NULL, NULL, NULL, ptr->parent);
        *sub2 = new(ptr->key[2], ptr->info->info[2], ptr->sub[2], ptr->sub[3], ptr->sub[4], NULL, NULL, ptr->parent);
        insert(sub2, ptr->key[3], ptr->info->info[3], ptr->parent);
        return;
}

void toParent(Tree **sub0, Tree **sub2){
        for (int i = 0; i<5; i++)
                if ((*sub0)->sub[i]) (*sub0)->sub[i]->parent = *sub0;
        for (int i = 0; i<5; i++)
                if ((*sub2)->sub[i]) (*sub2)->sub[i]->parent = *sub2;
}

Tree *cop(Tree *tree){
        Tree *ptr = malloc(1*sizeof(Tree));
        ptr->info = malloc(1*sizeof(Info));
        for (int i = 0; i<4; i++){
                ptr->key[i] = tree->key[i];
                ptr->info->info[i] = tree->info->info[i];
                ptr->sub[i] = tree->sub[i];
        }
        ptr->sub[4] = tree->sub[4];
        ptr->parent = tree->parent;
        ptr->size = tree->size;
        return ptr;
}

void split(Tree **TREE, Tree *tree){
        if (tree==NULL) return;
        if (tree->size<4) return;
        Tree *ptr = cop(tree), *sub0 = NULL, *sub1 = NULL, *sub2 = NULL, *sub3 = NULL, *sub4 = NULL, *par = NULL, *parent = NULL, *PAR = NULL;

        int subD = parf(tree, &parent);

        sub(&sub0, &sub2, ptr);

        if (!parent){
                par = new(ptr->key[1], ptr->info->info[1], sub0, sub2, sub1, sub3, sub4, NULL);
                sub0->parent = par;
                sub2->parent = par;
                toParent(&sub0, &sub2);
                free(ptr->info);
                free(ptr);
                free((*TREE)->info);
                free(*TREE);
                *TREE = par;
                return;
        } else {
//              help(parent);
//              help(parent->sub[subD]);
                free(parent->sub[subD]->info);
                free(parent->sub[subD]);
                parent->sub[subD] = NULL;
                if (ptr->key[1]<=parent->key[0]){
                        toParent(&sub0, &sub2);
                        for (int i = 4; i>1; i--) parent->sub[i] = parent->sub[i-1];
                        parent->sub[1] = sub2;
                        parent->sub[0] = sub0;
                } else if ((parent->size>1 && ptr->key[1]<=parent->key[1]) || (parent->size==1)){
                        toParent(&sub0, &sub2);
                        help(sub0);
                        help(sub2);

                        for (int i = 4; i>2; i--) parent->sub[i] = parent->sub[i-1];
                        parent->sub[2] = sub2;
                        parent->sub[1] = sub0;
                } else if ((parent->size>2 && ptr->key[1]<=parent->key[2]) || (parent->size==2)){
                        toParent(&sub0, &sub2);
                        for (int i = 4; i>3; i--) parent->sub[i] = parent->sub[i-1];
                        parent->sub[3] = sub2;
                        parent->sub[2] = sub0;
                } else {
                        toParent(&sub0, &sub2);
                        parent->sub[3] = sub0;
                        parent->sub[4] = sub2;
                }
                insert(&parent, ptr->key[1], ptr->info->info[1], parent->parent);
                free(ptr->info);
                free(ptr);
                split(TREE, parent);
        }
        return;
}

void help(Tree *ptr){
/*      if (!ptr) return;
        printf("ptr: %d[%d, %d, %d, %d]\n", ptr->parent, ptr->key[0], ptr->key[1], ptr->key[2], ptr->key[3]);
        if (ptr->parent) printf("parent: [%d, %d, %d, %d]\n", ptr->parent->key[0], ptr->parent->key[1], ptr->parent->key[2], ptr->parent->key[3]);
        for (int i = 0; i<5; i++){
                printf("sub%d: %d\n", i, ptr->sub[i]);
                if (ptr->sub[i]) printf("sub%d: [%d, %d, %d, %d]\n", i, ptr->sub[i]->key[0], ptr->sub[i]->key[1], ptr->sub[i]->key[2], ptr->sub[i]->key[3]);
        }*/
}

//----------------------------- DELETE BEGIN -----------------------------------

void shift(Tree **ptr, int sub){
        for (int i = sub; i<3; i++) (*ptr)->key[i] = (*ptr)->key[i+1];
}

void shiftGROU(Tree **ptr, int sub){
        for (int i = 3; i>sub; i--) (*ptr)->key[i] = (*ptr)->key[i-1];
}

void shiftSub(Tree **ptr, int sub){
        for (int i = sub; i<4; i++) (*ptr)->sub[i] = (*ptr)->sub[i+1];
}

void shiftSubGROU(Tree **ptr, int sub){
        for (int i = 4; i>sub; i--) (*ptr)->sub[i] = (*ptr)->sub[i-1];
}

void delLeaf(Tree **ptr, Tree *parent, int key, int sub){
        int i, j;
        for (i = 0; i<3; i++){
                if ((*ptr)->key[i]==key){
                        (*ptr)->key[i] = 0;
                        break;
                }
        }
        ((*ptr)->size)-=1;
        shift(ptr, i);
        if ((*ptr)->size==0){
                if (parent){
                        free(parent->sub[sub]->info);
                        free(parent->sub[sub]);
                        parent->sub[sub] = NULL;
                } else {
                        free((*ptr)->info);
                        free(*ptr);
                }
        }
}

void perestr(Tree **TREE, Tree *ptr){
        if (ptr->size>0){
                printf("end\n");
                return;
        }
        int sub;
        Tree *SUB;
        for(sub = 0; sub<4; sub++){
                if (ptr->parent->sub[sub] && ptr->parent->sub[sub]->size==0) break;
        }
        int subPar, leaf, subLeaf, KEY, KEYhead, keysiz, subsiz;
        Tree *p, *parent = ptr->parent;
        if (sub>0){
                subPar = sub-1;
                leaf = sub-1;
                keysiz = parent->sub[leaf]->size-1;
                subsiz = parent->sub[leaf]->size;
        } else {
                subPar = sub;
                leaf = sub+1;
                keysiz = 0;
                subsiz = 0;
        }
        if (parent->sub[leaf]->size>1){
                KEY = parent->sub[leaf]->key[keysiz];
                SUB = parent->sub[leaf]->sub[subsiz];
                help(SUB);
                printf("%d\n", subsiz);
                parent->sub[leaf]->sub[subsiz] = NULL;
                parent->sub[leaf]->key[keysiz] = 0;
                parent->sub[leaf]->size -= 1;
                if (!sub>0) shift(&(parent->sub[leaf]), 0);
                if (sub>0) KEYhead = parent->key[0];
                else KEYhead = parent->key[parent->size-1];
                parent->key[0] = KEY;
                parent->sub[sub]->key[0] = KEYhead;
                parent->sub[sub]->size += 1;
                if (sub>0){
                        shiftSubGROU(&(parent->sub[sub]), 0);
                        parent->sub[sub]->sub[0] = SUB;
                        if(parent->sub[sub]->sub[0]) parent->sub[sub]->sub[0]->parent = parent->sub[sub];
                }
                else {
                        parent->sub[sub]->sub[parent->size] = SUB;
                        if (parent->sub[sub]->sub[parent->size]) parent->sub[sub]->sub[parent->size]->parent = parent->sub[sub];
                }
                return;
        } else {
                SUB = parent->sub[sub]->sub[0];
                if (sub>0) parent->sub[leaf]->key[parent->sub[leaf]->size] = parent->key[0];
                else {
                        shiftGROU(&(parent->sub[leaf]), 0);
                        parent->sub[leaf]->key[0] = parent->key[0];
                }
                parent->sub[leaf]->size+=1;
                if (sub>0){
                parent->sub[leaf]->sub[subsiz+1] = SUB;
                parent->sub[leaf]->sub[subsiz+1]->parent = parent->sub[leaf];
                } else{
                parent->sub[leaf]->sub[0] = SUB;
                parent->sub[leaf]->sub[0]->parent = parent->sub[leaf];
                }
                free(parent->sub[sub]->info);
                free(parent->sub[sub]);
                parent->sub[sub] = NULL;
                shift(&(parent), 0);
                parent->size -= 1;
                if (parent->size==0 && !parent->parent){
                        Tree *e = *TREE;
                        if (parent->sub[0]) *TREE = parent->sub[0];
                        if (parent->sub[1]) *TREE = parent->sub[1];
                        if (parent->sub[2]) *TREE = parent->sub[2];
                        if (parent->sub[3]) *TREE = parent->sub[3];
                        free(e->info);
                        free(e);
                        (*TREE)->parent = NULL;
                        return;
                }
                perestr(TREE, parent);
        }
        return;
}

void delLeaf1(Tree **TREE, Tree **ptr, Tree *parent, int key, int sub){
        int subPar, leaf, subLeaf;
        Tree *p;
        if (parent->sub[sub-1] && sub>0 && parent->sub[sub-1]->size>1){
                subPar = sub-1;
                leaf = sub-1;
                subLeaf = parent->sub[sub-1]->size;
        } else if (parent->sub[sub+1] && sub<3 && parent->sub[sub+1]->size>1){
                subPar = sub;
                leaf = sub+1;
                subLeaf = 0;
        }
        else {
                if (sub>0){
                        subPar = sub-1;
                        leaf = sub-1;
                } else {
                        subPar = sub;
                        leaf = sub+1;
                }
                parent->sub[leaf]->key[1] = parent->key[subPar];
                (parent->sub[leaf]->size)+=1;
                shift(&parent, subPar);
                free(parent->sub[sub]->info);
                free(parent->sub[sub]);
                (parent->sub[sub]) = NULL;
                parent->size-=1;
                shiftSub(&parent, sub);
                if (sub>0) p = parent->sub[leaf];
                else p = parent->sub[leaf-1];
                increase(p->size, &(p->key[0]), &(p->key[1]), &(p->key[2]), &(p->key[3]), &(p->info->info[0]), &(p->info->info[1]), &(p->info->info[2]), &(p->info->info[3]));
                perestr(TREE, parent);
                return;
        }
        (*ptr)->key[0] = parent->key[subPar];
        if (!subLeaf){ parent->key[subPar] = parent->sub[leaf]->key[subLeaf];
        parent->sub[leaf]->key[subLeaf] = parent->sub[leaf]->key[subLeaf+1];
        parent->sub[leaf]->key[subLeaf+1] = 0;
        }
        else{ parent->key[subPar] = parent->sub[leaf]->key[subLeaf-1];
        parent->sub[leaf]->key[subLeaf-1] = 0;
        }
        (parent->sub[leaf]->size)-=1;
        increase((*ptr)->size, &((*ptr)->key[0]), &((*ptr)->key[1]), &((*ptr)->key[2]), &((*ptr)->key[3]), &((*ptr)->info->info[0]), &((*ptr)->info->info[1]), &((*ptr)->info->info[2]), &((*ptr)->info->info[3]));
        perestr(TREE, parent);
        return;
}

int parf(Tree *ptr, Tree **parent){
        int sub = 0;
        *parent = ptr->parent;
        if (!(*parent)) return -1;
        for (int k = 0; k<4; k++){
                if ((*parent)->sub[k]==ptr){
                        sub = k;
                        break;
                }
        }
        return sub;
}

int preLast(Tree *ptr){
        for (int i = 0; i<4; i++){
                if (ptr->sub[i] && !isLeaf(ptr->sub[i])){
                        return 0;
                }
        }
        return 1;
}

int findLeafsub(Tree *ptr, int key){
        for (int i = 0; i<3; i++){
                if (ptr->key[i]==key){
                        return i;
                }
        }
        return -1;
}
Tree *findMax(Tree *tree, Tree **par, int *sub){
        int i = 3;
        if (!tree) return NULL;
        while (!isLeaf(tree)){
                i = 3;
                while (!(tree->sub[i])) i--;
                *sub = i;
                *par = tree;
                tree = tree->sub[i];
        }
        return tree;
}

void deleteVer(Tree **TREE, Tree *tree, int key){
        int i, sub;
        int ind = findLeafsub(tree, key);
        Tree *par = NULL, *ptr = findMax(tree->sub[ind], &par, &sub);
        for (i = 0; i<3; i++){
                if (tree->key[i]==key){
                        tree->key[i] = ptr->key[ptr->size-1];
                        del(TREE, ptr->key[ptr->size-1]);
                        break;
                }
        }
        return;
}

void del(Tree **tree, int key){
        int sub = 0;
        Tree *pred = NULL, *ptr = NULL, *parent;
        ptr = find(*tree, key, &pred);
        if (!ptr){
                if (!pred){
                        return;
                } else {
                        ptr = pred;
                }
        }
        if (ptr->parent){
                sub = parf(ptr, &parent);
        }
        if (isLeaf(ptr) && ptr->size>1){
                delLeaf(&ptr, parent, key, sub);
        } else if (isLeaf(ptr)){
                delLeaf1(tree, &ptr, parent, key, sub);
        } else if (preLast(ptr)){
                splitDelpreLast(&ptr, key);
        } else {
                deleteVer(tree, ptr, key);
        }
        return;
}

Tree **splitDelpreLast(Tree **tree, int key){
        Tree *ptr = *tree;
        int sub = findLeafsub(*tree, key);
        if (ptr->sub[sub+1]){
                ptr->key[sub] = ptr->sub[sub+1]->key[0];
                (ptr->sub[sub+1]->size)-=1;
                shift(&(ptr->sub[sub+1]), 0);
                if (ptr->sub[sub+1]->size==0 && sub>0){
                        (ptr->sub[sub+1]->size)+=1;
                        ptr->sub[sub+1]->key[0] = ptr->key[sub-1];
                        splitDelpreLast(&ptr, ptr->key[sub-1]);
                } else if (ptr->sub[sub+1]->size==0){
                        free(ptr->sub[sub+1]->info);
                        free(ptr->sub[sub+1]);
                        ptr->sub[sub+1] = NULL;
                }
        } else if (sub>0){
                (ptr->size)-=1;
                shift(&ptr, sub);
                shiftSub(&ptr, sub+1);
        } else {
                ptr->key[0] = ptr->sub[0]->key[0];
                free(ptr->sub[0]->info);
                free(ptr->sub[0]);
                ptr->sub[0] = NULL;
        }
}

//----------------------------- DELETE END -----------------------------------

Tree *SpecialFindBec(Tree *tree){
        if (tree == NULL){
                return NULL;
        }
        Tree *cmp = tree;
        for (int i = 0; i < 4; i++){
                if (tree->sub[i]!=NULL){
                        cmp = tree->sub[i];
                        break;
                }
        }
        while (cmp->sub[0] != NULL){
                cmp = cmp->sub[0];
        }
        return cmp;
}

void BypassFunc(Tree *tree){
        if (tree == NULL){
                return;
        }
        BypassFunc(tree->sub[3]);
        BypassFunc(tree->sub[2]);
        printf("%d ", tree->key[0]);
        if (tree->key[1]!=0) printf("%d ", tree->key[1]);
        if (tree->key[2]!=0) printf("%d ", tree->key[2]);
        BypassFunc(tree->sub[1]);
        BypassFunc(tree->sub[0]);
}

void show(Tree *tree, int level){
        int i = level;
        if (tree){
                show(tree->sub[3], level+1);
                show(tree->sub[2], level+1);
                while (i-- > 0){
                        printf("         ");
                }
                if (tree->parent) printf("%d[%d, %d, %d]%d\n", tree->parent->key[0], tree->key[0], tree->key[1], tree->key[2], tree->size);
                else printf("[%d, %d, %d]%d\n", tree->key[0], tree->key[1], tree->key[2], tree->size);
                show(tree->sub[1], level+1);
                show(tree->sub[0], level+1);
        }
}

void clearn(Tree **tree){
        if (*tree){
                clearn(&((*tree)->sub[3]));
                clearn(&((*tree)->sub[2]));
                clearn(&((*tree)->sub[1]));
                clearn(&((*tree)->sub[0]));
                free((*tree)->info);
                free(*tree);
        }
}
