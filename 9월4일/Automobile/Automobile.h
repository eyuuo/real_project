class Automobile {
private:    
    float fuel;
    float speed;
    char licensePlate[32];

public:
    Automobile();
    void Display();
    void Accelerate(int amount);
    void Decelerate(int amount);
    void Stop();//
};
