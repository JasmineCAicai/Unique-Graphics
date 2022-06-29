# Halfedge Mesh

For an edge connecting two vertices i and j, one of its halfedges points from i to j; the other one points from j to i.
- The two halfedges are oppositely oriented.
- One of the halfedges is associated with the face to the “left” of the edge; the other is associated with the face to the “right”.
- twin: the opposite halfedge

In contrast, the standard mesh elements (vertices, edges, and faces) know only about one of their halfedges. In particular:
- a vertex knows about one of its “outgoing” halfedges
- an edge knows about one of its two halfedges
- a face knows about one of the many halfedges circulating around its interior

In summary, we have the following relationships:
| Mesh Element | Pointers |
|--|--|
| Vertex | halfedge (just one) |
|Edge | halfedge (just one) |
| Face | halfedge (just one) |
| Halfedge | next, twin, vertex, edge, face |

This list emphasizes that it is really the halfedges that connect everything up. 

For example, suppose we have a face f and want to print out the positions of all its vertices. We would write a routine like this:
```cpp
void printVertexPositions(FaceRef f) {
	HalfedgeRef h = f->halfegde();	  // get the first halfedge of the face
	do {
		VertexRef v = h->vertex();	    // get the vertex of the current halfedge
		cout << v->pos << endl;	        // print the vertex position
		h = h->next();			            // move to the next halfedge around the face
	} while (h != f->halfedge());		  // keep going until we’re back at the beginning
}
```

Similarly, to print out the positions of all the neighbors of a given vertex we could write a routine like this:
```cpp
void printNeighborPositions(VertexRef v) {
	HalfedgeRef h = v->halfedge();	      // get one of the outgoing halfedge of the vertex
	do {
		HalfedgeRef h_twin = h->twin();	    // get the vertex of the current halfedge
		VertexRef vN = h_twin->vertex();	  // vertex is ‘source’ of the half edge
							                          // so h->vertex() is v
						                            // whereas h_twin->vertex() is the neighbor vertex
		cout << vN->pos << endl;	          // print the vertex position
		h = h_twin->next();		              // move to the next outgoing halfedge of the vertex
	} while(h != v->halfedge());		      // keep going until we’re back at the beginning
}
```

To iterate over all the vertices in a halfedge mesh, we could write a loop like this:
```cpp
for(VertexRef v = vertices_begin(); v != vertices_end(); v++) {
	printNeighborPositions(v);
}
```

Need to consider whether the edge is the boundary.
