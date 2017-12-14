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

Meshes are by far the most common representation of surfaces in games. <!-- .element: class="fragment current-visible" -->

A total pain in the ass... <!-- .element: class="fragment fade-up" -->

But well understood. <!-- .element: class="fragment fade-up" -->

---

<!-- .slide: data-background="mesh.png" -->

## B-Reps

<large>*More than a graphical tool*</large>

<image style="height: 200px;" src="convex_hull.jpg"><image style="height: 200px;" src="FEA.jpg">

<span class="fragment">
	Convex hull generation<br><small>*(collision systems, physics)*</small>
</span>

Navigation / Mapping <!-- .element: class="fragment" -->

Finite element simulation / analysis <!-- .element: class="fragment" -->

Robotics <!-- .element: class="fragment" -->

---

<!-- .slide: data-background="mesh.png" -->

## Other common b-reps

Winged edge <!-- .element: class="fragment fade-up" -->

Quad edge <!-- .element: class="fragment fade-up" -->

Tetrahedral mesh <!-- .element: class="fragment fade-up" -->

["triangle data structure"](http://sandervanrossen.blogspot.de/2017/09/half-edge-data-structure-considered.html) <!-- .element: class="fragment fade-up" -->

---

<!-- .slide: data-background="mesh.png" -->

## Quick Vocab Recap

- Boundary Edge <span class="fragment"><br>*An edge with no neighbor.*</span>

- Regular Edge <span class="fragment"><br>*An edge with one neighbor.*</span>

- Singular Edge <span class="fragment"><br>*An edge with more than one neighbor.*</span>

---

<!-- .slide: data-background="mesh.png" -->

## Quick Vocab Recap

- Closed Mesh <span class="fragment current-visible"><br>*A mesh with no boundary edges.*</span>

- Manifold Mesh <span class="fragment"><br>*A mesh with no singular edges.*</span>

---

<!-- .slide: data-background="mesh.png" -->

## Half-Edge Meshes

<image src="00-edge.jpg" style="height: 400px">

<span class="fragment">Let's build a small library for manifold meshes.</span>

<span class="fragment">But first we'll walk through some essentials in psuedo-c++</span>

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

<small>As you may have guessed, the edge is our primary adjacency structure.<br>It is our means for traversing a mesh in a meaningful way.</small>

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

<small>Let's quickly explore how to circulate a face.</small>

---

<!-- .slide: data-background="mesh.png" -->

## Anatomy of a triangle

<image src="01-traversal-01.jpg" style="height: 400px">

<small>Ignoring some details, let's start from the face and move to the first edge.</small>

```cpp
edge_t* edge = face->edge;
```

---

<!-- .slide: data-background="mesh.png" -->

## Anatomy of a triangle

<image src="01-traversal-02.jpg" style="height: 400px">

<small>Next we travel to the next edge in the loop.</small>

```cpp
edge = edge->next;
```

---

<!-- .slide: data-background="mesh.png" -->

## Anatomy of a triangle

Knowing what we know now, let's calculate the normal for a vertex:

```cpp
vec3 calc_normal(vertex_t* vert) {
	auto* edge = vert->edge;
	auto* p0 = edge->vertex->point;
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

## Anatomy of a triangle

Calculating the area is almost the idential operation:

```cpp
float calc_area(vertex_t* vert) {
	auto* edge = vert->edge;
	auto* p0 = edge->vertex->point;
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
Our half-edge mesh library could provide iterators and circulators to allow application developers to easily traverse meshes and accumulate values.
</span>

---

<!-- .slide: data-background="mesh.png" -->

### Iterate over components of a face

<small>The face number could be the average of all vertex normals.</small>

```cpp
vec3 normal;
int vert_total = 1;
for (auto* vert : mesh.iter_verts()) {
	normal += calc_normal(vert);
}
normal = normal / vert_total;
```

---

<!-- .slide: data-background="mesh.png" -->

### Iterate over components of a face

Something is fishy with that area calculation though.

<span class="fragment current-visible">
Seems you'll end up with overlapping regions being factored into the area.
</span>

<span class="fragment current-visible">
So you can't just iterate over every edge in the face to make that calculation.
</span>

---

<!-- .slide: data-background="mesh.png" -->

### Iterate over components of a face

You could build a suite of iterators for these cases. Let's just use a while loop for now:

```cpp
edge_t* edge = face->edge;
while ()
```

---

## Intergalactic Interconnections

You can link between slides internally, [like this](#/2/3).

---

## Speaker View

There's a [speaker view](https://github.com/hakimel/reveal.js#speaker-notes). It includes a timer, preview of the upcoming slide as well as your speaker notes.

Press the _S_ key to try it out.

<aside class="notes">Oh hey, these are some notes. They'll be hidden in your presentation, but you can see them if you open the speaker notes window (hit 's' on your keyboard).</aside>
