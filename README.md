# DelaunayUplifting
Create Delaunay triangular mesh with lifting method

The methods of constructing Delaunay triangular mesh mainly include adding point method and lifting method. The idea of the lifting method is to raise the two-dimensional point set to the three-dimensional space and construct the convex hull of the three-dimensional point set, which is easily obtained by physical intuition. Looking up from the xy plane, the triangular piece that we can "see" is projected and obtained. It should be the Delaunay triangle mesh of the point set.

uplift point(x,y) to 3D zone by z = sqrt(x* x + y* y) and create the convex hull of the 3D points
like this
![Image text](https://github.com/XinShiOfficial/DelaunayUplifting/blob/master/res/convex1.png)
![Image text](https://github.com/XinShiOfficial/DelaunayUplifting/blob/master/res/convex2.png)

 Looking up from the xy plane, the triangular piece that we can "see" is projected and obtained
 that is Delaunay Triangle mesh
 ![Image text](https://github.com/XinShiOfficial/DelaunayUplifting/blob/master/res/dt.png)
