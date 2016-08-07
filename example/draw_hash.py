
def draw_box( ax,p0,p1,color='w' ):

    ax.plot( [p0[0],p1[0]] , [p0[1],p0[1]], [p0[2],p0[2]],color)
    ax.plot( [p1[0],p1[0]] , [p0[1],p0[1]], [p0[2],p1[2]],color)
    ax.plot( [p1[0],p0[0]] , [p0[1],p0[1]], [p1[2],p1[2]],color)
    ax.plot( [p0[0],p0[0]] , [p0[1],p0[1]], [p1[2],p0[2]],color)
    
    ax.plot( [p0[0],p1[0]] , [p1[1],p1[1]], [p0[2],p0[2]],color)
    ax.plot( [p1[0],p1[0]] , [p1[1],p1[1]], [p0[2],p1[2]],color)
    ax.plot( [p1[0],p0[0]] , [p1[1],p1[1]], [p1[2],p1[2]],color)
    ax.plot( [p0[0],p0[0]] , [p1[1],p1[1]], [p1[2],p0[2]],color)

    ax.plot( [p0[0],p0[0]] , [p1[1],p0[1]], [p0[2],p0[2]],color)
    ax.plot( [p1[0],p1[0]] , [p1[1],p0[1]], [p0[2],p0[2]],color)

    ax.plot( [p0[0],p0[0]] , [p1[1],p0[1]], [p1[2],p1[2]],color)
    ax.plot( [p1[0],p1[0]] , [p1[1],p0[1]], [p1[2],p1[2]],color)


def draw_particle(ax,x,y,z):

    ax.scatter(x,y,z)
