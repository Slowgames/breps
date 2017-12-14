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

- Closed Mesh <span class="fragment current-visible"><br>*A mesh with no boundary edges.*</span>

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
Our half-edge mesh library should provide iterators and circulators that allow application developers to easily traverse meshes and accumulate values.
</span>

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
