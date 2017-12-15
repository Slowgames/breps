---
theme : "black"
transition: "none"
highlightTheme: "atom-one-dark"
---

<!-- .slide: data-background="mesh.png" -->

# Half-Edge Mesh

### A fantastically useful data structure for procedural modeling.

---

<!-- .slide: data-background="mesh.png" -->

## B-Reps

*"Boundary Representation"* <!-- .element: class="fragment" -->

A total pain in the ass... <!-- .element: class="fragment fade-up" -->

But well understood. <!-- .element: class="fragment fade-up" -->

---

<!-- .slide: data-background="mesh.png" -->

## B-Reps

<large>*More than a graphical tool*</large>

<image style="height: 200px;" src="convex_hull.jpg"><image style="height: 200px;" src="FEA.jpg">

Convex hull generation<br><small>*(collision systems, physics)*</small>

Navigation / Mapping

---

<!-- .slide: data-background="mesh.png" -->

## B-Reps

<large>*More than a graphical tool*</large>

<image style="height: 200px;" src="convex_hull.jpg"><image style="height: 200px;" src="FEA.jpg">

Finite element simulation / analysis

Robotics

---

<!-- .slide: data-background="mesh.png" -->

## Other common b-reps

Winged edge

Quad edge

Tetrahedral mesh

["triangle data structure"](http://sandervanrossen.blogspot.de/2017/09/half-edge-data-structure-considered.html)

---

<!-- .slide: data-background="mesh.png" -->

### Terms that may get thrown around

- Boundary Edge <span class="fragment"><br>*An edge with no neighbor.*</span>

- Regular Edge <span class="fragment"><br>*An edge with one neighbor.*</span>

- Singular Edge <span class="fragment"><br>*An edge with more than one neighbor.*</span>

---

<!-- .slide: data-background="mesh.png" -->

### Terms that may get thrown around

- Closed Mesh <span class="fragment"><br>*A mesh with no boundary edges.*</span>

- Manifold Mesh <span class="fragment"><br>*A mesh with no singular edges.*</span>

---

<!-- .slide: data-background="mesh.png" -->

## Half-Edge Meshes

<image src="00-edge.jpg" style="height: 400px">

<span class="fragment">First a tour through an imaginary C++ API.</span>

---

<!-- .slide: data-background="mesh.png" -->

## Essential Structures

```cpp
struct edge_t {
  vertex_t* vertex;
  face_t* face;
  edge_t* next;
  edge_t* prev;
  edge_t* adjacent;
};
```

<small>The edge is our primary adjacency structure. <br>It is a switch board for fast mesh traversals.</small>

---

<!-- .slide: data-background="mesh.png" -->

## Essential Structures

```cpp
struct face_t {
  edge_t* edge;
};
```

<small>Our face structure stores only a reference to the root of it's edge loop.<br>Some applications may want to store additional information here.</small>

---

<!-- .slide: data-background="mesh.png" -->

## Essential Structures

```cpp
struct vertex_t {
  point_t* point;
  edge_t* edge;
};
```

<small>Vertices are the root of an edge and associate the edge with points.</small>

---

<!-- .slide: data-background="mesh.png" -->

## Essential Structures

```cpp
struct point_t {
  vec3 position;
};
```

<small>Points are where we store our various attributes such as position and color.</small>

---

<!-- .slide: data-background="mesh.png" -->

## Anatomy of a triangle

<image src="00-triangle.jpg" style="height: 400px">

---

<!-- .slide: data-background="mesh.png" -->

## Anatomy of a triangle

<image src="01-traversal-01.jpg" style="height: 400px">

```cpp
edge_t* edge = face->edge;
```

---

<!-- .slide: data-background="mesh.png" -->

## Anatomy of a triangle

<image src="01-traversal-02.jpg" style="height: 400px">

```cpp
edge = edge->next;
```

---

<!-- .slide: data-background="mesh.png" -->

## Anatomy of a triangle

Armed with this visualization in mind let's sketch two fundamental calculations...

---

<!-- .slide: data-background="mesh.png" -->

## Vertex Normals

Use the cross product of it's connected edges:

```cpp
vec3 calc_normal(vertex_t* vert) {
	auto* edge = vert->edge;
	auto* p0 = vert->point;
	auto* p1 = edge->next->vertex->point;
	auto* p2 = edge->prev->vertex->point;

	auto A = p1->position - p0->position;
	auto B = p2->position - p0->position;

	return A.cross(B).normalized();
}

auto normal = calc_normal(face->edge->vertex);
```

---

<!-- .slide: data-background="mesh.png" -->

## Area of the triangle

Nearly identical to the normal calculation:

```cpp
float calc_area(vertex_t* vert) {
	auto* edge = vert->edge;
	auto* p0 = vert->point;
	auto* p1 = edge->next->vertex->point;
	auto* p2 = edge->next->next->vertex->point;

	auto A = p1->position - p0->position;
	auto B = p2->position - p0->position;

	return A.cross(B).length() / 2;
}

auto area = calc_area(face->edge->vertex);
```

---

<!-- .slide: data-background="mesh.png" -->

### What about polygons with N > 3 vertices?

<h1 class="fragment current-visible">
Madness!!!
</h1>

<span class="fragment">
Our half-edge mesh library should provide iterators and circulators for common traversals.
</span>

<span class="fragment">
For some algorithms, you'll still want to be able to traverse with while and for loops though.
</span>

---

<!-- .slide: data-background="mesh.png" -->

### Circu-wha?

How do I iterate over all of the out-going edges from a point?

---

<!-- .slide: data-background="mesh.png" -->

### Circu-wha?

<image src="04-circulator-01.jpg" style="height: 300px">

```cpp
auto* edge = vert->edge;
```

---

<!-- .slide: data-background="mesh.png" -->

### Circu-wha?

<image src="04-circulator-02.jpg" style="height: 300px">

```cpp
edge = edge->prev->adjacent;
```

---

<!-- .slide: data-background="mesh.png" -->

### Face Normals

<small>The face normal is the average of all vertex normals.</small>

```cpp
vec3 normal;
int vert_total = 1;
for (auto* vert : mesh.iter_verts()) {
	normal += calc_normal(vert);
	++vert_total;
}
normal = normal / vert_total;
```

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

Calculating the area of a polygon is a bit trickier.

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

You can't just iterate over every vert in the face because you'll end up with overlapping regions.<br>
<image src="03-area-04.jpg" style="height: 300px">

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

Instead you have to fan out over a potential triangulation of the polygon.<br>
<image src="03-area-05.jpg" style="height: 300px">

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

Which means we need to also modify our area calculation:

```cpp
float calc_area(point_t* p0, point_t* p1, point_t* p2) {
	auto A = p1->position - p0->position;
	auto B = p2->position - p0->position;

	return A.cross(B).length() / 2;
}
```

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

Sum the areas of the triangles to get the total area:

```cpp
vert_t* next_vert(vert_t* vert) {
	return vert->edge->next->vertex;
}

float area = 0.f;
vert_t* v0 = face->edge->vertex;
vert_t* v1 = next_vert(v0);
vert_t* v2 = next_vert(v1);
while(v2 != v0) {
	area += calc_area(v0->point, v1->point, v2->point);
	v1 = v2;
	v2 = next_vert(v1);
}
```

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

<image src="03-area-01.jpg" style="height: 400px">

A<sup>0</sup> = *area*(v<sup>0</sup>, v<sup>1</sup>, v<sup>2</sup>)

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

<image src="03-area-02.jpg" style="height: 400px">

A<sup>1</sup> = *area*(v<sup>0</sup>, v<sup>2</sup>, v<sup>3</sup>)

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

<image src="03-area-03.jpg" style="height: 400px">

A<sup>2</sup> = *area*(v<sup>0</sup>, v<sup>3</sup>, v<sup>4</sup>)

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

What about CONVEX polygons!?

<image src="03-convex-area-01.jpg" style="height: 400px">

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

No problem!

<image src="the_hoff.jpg" style="height: 400px">

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

<image src="03-convex-area-02.jpg" style="height: 400px">

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

<image src="03-convex-area-03.jpg" style="height: 400px">

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

<image src="03-convex-area-04.jpg" style="height: 400px">

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

<image src="03-convex-area-05.jpg" style="height: 400px">

---

<!-- .slide: data-background="mesh.png" -->

### Area of an n-gon

<image src="no_problem.jpg" style="height: 400px">

---

<!-- .slide: data-background="mesh.png" -->

### What's the catch?

Mesh implementations have to take a lot into consideration:

- What sort of geometry do you want to support? <!-- .element: class="fragment" -->

- How can my iterators be robust in the face of non-compliant topology? <!-- .element: class="fragment" -->

- What comes first: the vertex or the edge? <!-- .element: class="fragment" -->
<br><small>This is a highly cyclical structure</small>

---

<!-- .slide: data-background="mesh.png" -->

### What's the catch?

Mesh implementations have to take a lot into consideration:

- What happens when you remove elements? <!-- .element: class="fragment" -->

- Will you support multi-threaded mutability? <!-- .element: class="fragment" -->
<br><small>:screaming:</small>

---

<!-- .slide: data-background="mesh.png" -->

### An example implementation in C++

https://github.com/photex/breps

Supporting manifold meshes with no limit on polygon order.

---

<!-- .slide: data-background="mesh.png" -->

### Library concepts

- Mesh: Top level container and access to elements

- Mesh Kernel: Fundamental storage interface.

- Function sets: Proxies over elements and the mesh kernel.

- Typesafe Indexes: Index types that constrain indices returned from being used for other element types.

---

<!-- .slide: data-background="mesh.png" -->

### Typesafe indices

Pointers are fine for illustration.

In practice this is a minefield. <!-- .element: class="fragment" -->

---

<!-- .slide: data-background="mesh.png" -->

### Typesafe indices

Use a struct to wrap the offset into a buffer, and the cell generation.

```cpp
enum class index_type_t : unsigned char {
  vertex, edge, face, point, unsupported
};
template<index_type_t TIndexType = index_type_t::unsupported>
struct index_t {
  offset_t offset;
  generation_t generation;
}
```

---

<!-- .slide: data-background="mesh.png" -->

### Typesafe indices

```cpp
struct edge_index_t : public index_t<index_type_t::edge>
{ using index_t::index_t; };

struct face_index_t : public index_t<index_type_t::face>
{ using index_t::index_t; };

struct vertex_index_t : public index_t<index_type_t::vertex>
{ using index_t::index_t; };

struct point_index_t : public index_t<index_type_t::point>
{ using index_t::index_t; };
```

---

<!-- .slide: data-background="mesh.png" -->

### Base type for all elements

```cpp
enum class element_status_t : uint16_t {
  ACTIVE = 0x0000,
  INACTIVE = 0x8000
};

struct element_t {
  element_status_t status; // Was this cell "removed"?
  uint16_t tag;            // Used by iterators
  uint32_t generation;     // Generation of the cell
};
```

---

<!-- .slide: data-background="mesh.png" -->

### "Function Sets" for fluent traversals

When you don't store pointers you lose some of the ease of traversal as seen in our examples.

Instead we can provide objects to act as proxies:

```cpp
mesh.edge(eindex0).next().vertex().point();
mesh.face(findex0).edge().next().next().vertex().point();
mesh.vertex(vindex0).edge().prev().adjacent();
```

---

<!-- .slide: data-background="mesh.png" -->

### Removing elements

Mark the cell as inactive and insert the index into a priority queue for re-use.

```cpp
mesh.kernel->remove(findex0);
assert(mesh.kernel->get(findex0) == nullptr);

auto findex1 = mesh.kernel->emplace(face_t {});
assert(findex1.offset == findex0.offset);
assert(findex1.generation > findex0.generation);
```

---

<!-- .slide: data-background="mesh.png" -->

### Building a triangle

```cpp
auto pindex0 = mesh.add_point(0.f, 0.f, 0.f);
auto pindex1 = mesh.add_point(1.f, 0.f, 0.f);
auto pindex2 = mesh.add_point(0.f, 1.f, 0.f);
```

---

<!-- .slide: data-background="mesh.png" -->

### Building a triangle

```cpp
auto findex0 = mesh.add_triangle(pindex0, pindex1, pindex2);
```

<image src="02-add_triangle-01.jpg" style="height: 300px">

---

<!-- .slide: data-background="mesh.png" -->

### Building a triangle

```cpp
auto pindex3 = mesh.add_point(1.f, 1.f, 0.f);
```

<image src="02-add_triangle-02.jpg" style="height: 300px">

---

<!-- .slide: data-background="mesh.png" -->

### Building a triangle

```cpp
auto adjacent_eindex =
  mesh.face(findex0).edge().next().adjacent().index();
auto findex1 = mesh.add_triangle(adjacent_eindex, pindex3);
```

<image src="02-add_triangle-04.jpg" style="height: 300px">

---

<!-- .slide: data-background="mesh.png" -->

### Show me some code!

#### Good time for questions :D
