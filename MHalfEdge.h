#ifndef MHALFEDGE_H_INCLUDED
#define MHALFEDGE_H_INCLUDED

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <GL/glut.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <set>
#include <unordered_set>
#pragma warning(disable:4996)
using namespace std;


template <class G>
struct Vertex {
	typedef typename G::edge E;
	int id;
	float x, y, z;
	E* edge;

	bool operator==(Vertex v) { return id == v.id; }
	void print() { cout << x << " " << y << " " << z << endl; }
	void vertexdraw();
};


template <class G>
struct Edge {
	typedef typename G::face F;
	typedef typename G::vertex V;
	int id;
	Edge* twin;
	Edge* next;
	V* head;
	V* tail;
	F* face;

	Edge(V* _t, V* _h) : tail(_t), head(_h) {};
	bool operator==(const Edge<G>& e)const { return (head == e.head) && (tail == e.tail); }
};

template <class G>
struct Face{
    typedef typename G::edge E;

    int id;
    E* edge;

    Face(E* _e):edge(_e){};
    bool operator==(Face f){id == f.id;}

	void draw();
	void drawEdge();
	void drawAdy();
};

template <class G>
class HalfEdgeM {
	typedef typename G::vertex v;
	typedef typename G::edge e;
	typedef typename G::face f;
	vector<v> verteces;
	vector<int> vIndex;
	vector<f> faces;
	unordered_set<e*> edges;
public:
	bool isTwin(e* e1, e* e2) { return (e1->tail == e2->head) && (e1->head == e2->tail); }
	v* getVertex(int i) { return &verteces[i - 1]; }
	void addFace2(v* v1, v* v2, v* v3) {
		//We check if edges already exist
		e* aux = new e(v1, v3);
		if (edges.find(aux) == edges.end()) {
			v1->edge = new e(v1, v3);
			edges.insert(v1->edge);
		}
		else {
			v1->edge = *edges.find(aux);
		}
		//For other two vertex
		delete aux;
		aux = new e(v3, v2);
		if (edges.find(aux) == edges.end()) {
			v1->edge->next = new e(v3, v2);
			edges.insert((v1->edge->next));
		}
		else {
			v1->edge->next = *edges.find(aux);
		}
		v2->edge = v1->edge->next;
		//Now other two vertex
		delete aux;
		aux = new e(v2, v1);
		if (edges.find(aux) == edges.end()) {
			v1->edge->next->next = new e(v2, v1);
			edges.insert((v1->edge->next->next));
		}
		else {
			v1->edge->next->next = *edges.find(aux);
		}
		v3->edge = v1->edge->next->next;
		//Now we create the half edges
		aux = new e(v1, v2);
		if (edges.find(aux) == edges.end()) {
			v1->edge->next->next->twin = new e(v2, v1);
			edges.insert((v1->edge->next->next->twin));
		}
		else {
			v1->edge->next->next->twin = *edges.find(aux);
		}
		delete aux;
		aux = new e(v2, v3);
		if (edges.find(aux) == edges.end()) {
			v1->edge->next->twin = new e(v2, v3);
			edges.insert((v1->edge->next->twin));
		}
		else {
			v1->edge->next->twin = *edges.find(aux);
		}
		delete aux;
		aux = new e(v3, v1);
		if (edges.find(aux) == edges.end()) {
			v1->edge->twin = new e(v3, v1);
			edges.insert((v1->edge));
		}
		else {
			v1->edge->twin = *edges.find(aux);
		}
		delete aux;
		//Now we attach the face
		//now we point the edges to the face
		v1->edge->face = new f(v1->edge);
		v1->edge->next->face = v1->edge->face;
		v1->edge->next->next->face = v1->edge->face;
		faces.push_back(*(v1->edge->face));
		//cout << "Creo cara" << endl;
	}
	bool loadOBJ(char* path) {
		FILE* file = fopen(path, "r");
		if (file == NULL) {
			cout << "No es posible abrir el archivo. Revisar path." << endl;
			return false;
		}
		while (1) {
			char line_header[128];

			int res = fscanf(file, "%s", line_header);
			if (res == EOF) break;
			if (strcmp(line_header, "v") == 0) {
				Vertex<G> v;
				fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
				verteces.push_back(v);
			}/*else if( strcmp(line_header,"vt") == 0 || strcmp(line_header,"vn") == 0){
				continue;
			}*/else if (strcmp(line_header, "f") == 0) {
				string v1, v2, v3;
				unsigned int verIndex[3], uvIndex[3], nIndex[3];
				//int matches = fscanf(file, "%d/ %d/ %d/\n", &verIndex[0], &verIndex[1], &verIndex[2]);
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &verIndex[0], &uvIndex[0], &nIndex[0], &verIndex[1], &uvIndex[1], &nIndex[1], &verIndex[2], &uvIndex[2], &nIndex[2]);
				if (matches != 9) {
					cout << "El archivo no puede ser leído. Revisar sección caras f" << endl;
					//return false;
				}
				vIndex.push_back(verIndex[0]);
				vIndex.push_back(verIndex[1]);
				vIndex.push_back(verIndex[2]);
			}
		}

		//Start building HE Mesh
		for (unsigned i = 0; i < vIndex.size(); i += 3) { //Start iterating over the vIndex
			//We point to the three triangle verteces
			v* v1 = &verteces[vIndex[i] - 1];
			v* v2 = &verteces[vIndex[i + 1] - 1];
			v* v3 = &verteces[vIndex[i + 2] - 1];
			addFace2(v1, v2, v3);
		}
	}

	void queryVertex(v* v1) {
		vector<f*> query;
		e* eaux = v1->edge;
		f* aux;
		do {
			aux = (eaux->face);
			query.push_back(aux);
			if (eaux->twin != NULL)
				eaux = eaux->twin->next;
			else break;
		} while (eaux != v1->edge);
		cout << query.size() << endl;
	}
    void print(){
		for (unsigned int i = 0; i < faces.size(); i++) {
			while (faces[i].edge == NULL) {
				i++;
			}
			faces[i].draw();
			faces[i].drawEdge();
		}
		//faces[6].drawAdy();
		//faces[14].drawAdy();
		//faces[190].drawAdy();
			verteces[200].vertexdraw();
        /*cout<<"vertices"<<endl;
        for(unsigned i=0;i<verteces.size();++i) verteces[i].print();
        cout<<"caras"<<endl;
        for(unsigned i=0;i<vIndex.size();i+=3) cout<<vIndex[i]<<" "<<vIndex[i+1]<<" "<<vIndex[i+2]<<endl;]*/

    }
};

class ToolBox {
public:
	typedef  Edge<ToolBox> edge;
	typedef  Face<ToolBox> face;
	typedef  Vertex<ToolBox> vertex;
};

namespace std
{
	template<>
	struct hash<Edge<ToolBox> >
	{
		size_t
			operator()(const Edge<ToolBox>*& obj) const
		{
			return hash<ToolBox::vertex*>()(obj->head);
		}
	};
}



#endif // MHALFEDGE_H_INCLUDED
