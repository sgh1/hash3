# hash3

This is a C++ class template for 3D spatial hashing.

It is a work in progress

### Usage

hash3 is designed to be used with any type that has a 3D vector type as a member.  hash3 and its support files will take care of the rest.

A usage example is provided in this repo.  For this example, consider a spatial hashing of particles, where a particle is as follows:

    class particle
    {
    public:

    /***
     * to work with hash3, the below needs to be
     * added -- one typedef and the two get_xyz
     * functions. The get_xyz only can return anything
     * that has T.x, T.y, T.z
     **/

    typedef double num_type;
    typedef myvect3d vect_type;

    static const vect_type& get_xyz(const particle& p){
        return p.m_r;
    }

    /***
     * end hash3 boilerplate
     **/

    particle(const vect_type& v, const vect_type& r,
        long id):
        m_r(r),m_v(v),m_idx(id)
    {}

    particle(const particle& x):
        m_r(x.m_r),m_v(x.m_v),m_idx(x.m_idx)
    {}

    particle(particle&& x):
        m_r(x.m_r),m_v(x.m_v),m_idx(x.m_idx)
    {}

    virtual ~particle(){};

    vect_type     m_r;
    vect_type     m_v;
    long int    m_idx;
    };

Note that vect_type, in this case, is defined by the client, ie, it is not necessary for the client code to change whatever 3D vector type it is already using.

I opted to require some boilerplate to the type to be hashed in favor of more complex C++ machinery.  I am open to simple suggestions which could allow hash3 to be used on classes that can't be modified, for whatever reason.

We can then create a hash of particles with something like:

    typedef myvect3d vect3_t;
    typedef hash3::int3<int>       idx_t;

    std::vector<particle> particles;
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<double> uniform_dist(0, 10.0);

    for(int i = 0; i < 200; i++)
    {
        particles.push_back( particle(  vect3_t(uniform_dist(e1),uniform_dist(e1),uniform_dist(e1) ),
                                        vect3_t(uniform_dist(e1),uniform_dist(e1),uniform_dist(e1) ), i ) );
    }

    hash3::hash3<particle> storage(std::move(particles), idx_t(5,5,5));
    
