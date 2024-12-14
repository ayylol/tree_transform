#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Vec3{
    double x,y,z;
};
struct Vec3 Neg(struct Vec3 a){
    struct Vec3 r={
        .x=-a.x,
        .y=-a.y,
        .z=-a.z
    };    
    return r;
}
struct Vec3 Add(struct Vec3 a, struct Vec3 b){
    struct Vec3 r={
        .x=a.x+b.x,
        .y=a.y+b.y,
        .z=a.z+b.z
    };    
    return r;
}
struct Vec3 Mul(double a, struct Vec3 b){
    struct Vec3 r={
        .x=a*b.x,
        .y=a*b.y,
        .z=a*b.z
    };    
    return r;
}
double dist(struct Vec3 a, struct Vec3 b){
    return sqrt(
            pow((a.x-b.x),2)+
            pow((a.y-b.y),2)+
            pow((a.z-b.z),2)
            );
}
struct Node{
    struct Vec3 p;
    struct Node* parent;
    size_t num_children;
    struct Node** children;
    size_t cap_children;
};

void print_tree(struct Node* n, int depth){
    printf("( %lf %lf %lf ) ", n->p.x,n->p.y,n->p.z);
    for (size_t i=0; i<n->num_children; ++i) {
        if(i!=n->num_children-1){printf("[ ");}
        print_tree(n->children[i], depth+1);
        if(i!=n->num_children-1){printf("] ");}
    }
}

struct Node* scan_node(){
    struct Node* n = malloc(sizeof(struct Node));
    // Y is up
    //scanf("( %lf %lf %lf )", &n->p.x, &n->p.y, &n->p.z);
    // Z is up
    scanf("( %lf %lf %lf )", &n->p.x, &n->p.z, &n->p.y);
    n->parent=NULL;
    n->num_children=0;
    n->cap_children=16;
    n->children=malloc(sizeof(struct Node*)*16);
    return n;
};
void add_child(struct Node* parent, struct Node* child){
    if(parent->num_children>=parent->cap_children){
        parent->cap_children*=2;
        parent->children=realloc(parent->children, sizeof(struct Node*)*parent->cap_children);
    }
    parent->children[parent->num_children]=child;
    parent->num_children++;
    child->parent=parent;
}

struct Node* scan_tree(){
    struct Node* last_split[10000];
    int tos=-1;
    struct Node* root = scan_node();
    struct Node* parent = root;
    char c;
    while((c=getchar())!=-1){
        if (c==' ') continue;
        if (c=='[') {
            assert(tos!=10000);
            last_split[++tos]=parent;
        }
        if (c==']') {
            assert(tos>=0);
            parent=last_split[tos--];
        }
        if (c=='('){
            ungetc(c, stdin);
            struct Node* n = scan_node();
            add_child(parent, n);
            parent=n;
        }
    }
    return root;
};

struct dist_res{int n; double d;} distance_dfs(struct Node* n){
    struct dist_res res = {.n=1,.d=dist(n->parent->p,n->p)};
    for (int i=0;i<n->num_children;++i){
        struct dist_res child_res = distance_dfs(n->children[i]);
        res.n+=child_res.n;
        res.d+=child_res.d;
    }
    return res;
}

double average_distance(struct Node* root){
    double d=0.0;
    int n=1;
    for (int i=0;i<root->num_children;++i){
        struct dist_res res = distance_dfs(root->children[i]);
        d+=res.d;
        n+=res.n;
    }
    return d/n;
}
void shrink(struct Node* n, double amount){
    n->p=Mul(amount, n->p);
    for (int i=0;i<n->num_children;++i){
        shrink(n->children[i],amount);
    }
}
void move(struct Node* n, struct Vec3 amount){
    n->p=Add(amount, n->p);
    for (int i=0;i<n->num_children;++i){
        move(n->children[i],amount);
    }
}

int main(){
    struct Node* root = scan_tree();
    move(root,Neg(root->p));
    shrink(root,0.0055/average_distance(root));
    //printf("%lf\n", average_distance(root));
    print_tree(root, 0);
    printf("\n");
}
