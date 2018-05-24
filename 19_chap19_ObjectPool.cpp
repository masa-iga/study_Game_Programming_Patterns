/*************************

  The code explains how to code Object Pool design pattern
  - class ParticlePool allocates class Particle in POOL_SIZE sizes
  - User can create a new Particle from ParticlePool::create()
  - Unused Particle object is handled in list

************************/

#include<cstdint>
#include<cstdio>

//#define NDEBUG
#include<cassert>

#define VPRINTF(...) do { \
        printf("V(l%d) %s: ", __LINE__, __func__); \
        printf(__VA_ARGS__); \
    } while(false) \


class Particle
{
public:
    Particle()
        : frameLeft_(0)
    {}

    void init(double x, double y,
            double xVel, double yVel, int lifetime);
    bool animate();
    bool inUse() const { return frameLeft_ > 0; }
    Particle* getNext() const { return state_.next; }
    void setNext(Particle* next) { state_.next = next; }

    double getX() const { return state_.live.x_; }
    double getY() const { return state_.live.y_; }

private:
    int frameLeft_;

    union
    {
        struct
        {
            double x_, y_, xVel_, yVel_;
        } live;

        Particle* next;
    } state_;

};


void Particle::init(double x, double y,
        double xVel, double yVel, int lifetime)
{
    VPRINTF("new particle initiated (%p)\n", this);

    state_.live.x_ = x;
    state_.live.y_ = y;
    state_.live.xVel_ = xVel;
    state_.live.yVel_ = yVel;
    frameLeft_ = lifetime;
}


bool Particle::animate()
{
    if (!inUse())   return false;

    frameLeft_--;
    state_.live.x_ += state_.live.xVel_;
    state_.live.y_ += state_.live.yVel_;

    if (frameLeft_ == 0)
        VPRINTF("the particle finished its lifetime (%p)\n", this);

    return frameLeft_ == 0;
}

/************************************************************/

class ParticlePool
{
public:
    ParticlePool();

    void create(double x, double y,
            double xVel, double yVel,
            int lifetime);

    void animate();
    void print() const;

private:
    static const int POOL_SIZE = 100;
    Particle particles_[POOL_SIZE];
    Particle* firstAvailable_;

};


ParticlePool::ParticlePool()
{
    firstAvailable_ = &particles_[0];

    for (int i = 0; i < POOL_SIZE - 1; i++)
    {
        particles_[i].setNext(&particles_[i+1]);
    }

    particles_[POOL_SIZE - 1].setNext(nullptr);

}


void ParticlePool::animate()
{
    for (int i = 0; i < POOL_SIZE; i++)
    {
        if (particles_[i].animate())
        {
            particles_[i].setNext(firstAvailable_);
            firstAvailable_ = &particles_[i];
        }
    }
}


void ParticlePool::create(double x, double y,
        double xVel, double yVel,
        int lifetime)
{

    assert(firstAvailable_ != nullptr);

    Particle* newParticle = firstAvailable_;
    firstAvailable_ = firstAvailable_->getNext();

    newParticle->init(x, y, xVel, yVel, lifetime);

}

void ParticlePool::print() const
{
    for (int i = 0; i < POOL_SIZE; i++)
    {
        if (particles_[i].inUse())
        {
            printf("particle %d: x = %.1lf  y = %.1lf (%p)\n",
                    i, particles_[i].getX(), particles_[i].getY(), &particles_[i]);
        }
    }
}

/************************************************************/
/*
    Special pool, only for specific object pool allocator
 */

class ParticleForPool2
{
public:
    // this class is only for ParticlePool2, object pool class
    friend class ParticlePool2;

private:
    // do not call me other than the specific object pool class
    ParticleForPool2()
        : inUse_(false)
    {}

    // this class knows this is contained in object pool class
    bool inUse_;
};


class ParticlePool2
{
public:
    ParticlePool2();

    void create(double x, double y,
            double xVel, double yVel,
            int lifetime);

    void animate();

private:
    static const int POOL_SIZE = 100;
    ParticleForPool2 pool_[POOL_SIZE];
    ParticleForPool2* firstAvailable_;
};

/************************************************************/
/*
   Generic pool & object pool
 */

template <class TObject>
class GenericPool
{
private:
    static const int POOL_SIZE = 100;

    TObject pool_[POOL_SIZE];
    bool inUse_[POOL_SIZE];
};

/************************************************************/
/*
   test
 */

void test_particlePool()
{
    ParticlePool particlePool;

    particlePool.create(0.0f, 1.0f, 2.0f, 3.0f, 5);
    particlePool.create(20.0f, 31.0f, 42.0f, 53.0f, 10);
    particlePool.create(120.0f, 131.0f, 142.0f, 153.0f, 15);

    int frame = 0;
    const int num_frame = 20;

    while (frame < num_frame)
    {
        VPRINTF("frame %d\n", frame);

        particlePool.animate();

        particlePool.print();

        frame++;
    }
}


void test_particleForPool2()
{
}


int main()
{
    test_particlePool();

    test_particleForPool2();

    return 0;
}
