export module observer;

// Observer interface - classes that want to be notified of changes
export class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update() = 0;
};

// Subject interface - classes that notify observers
export class ISubject {
public:
    virtual ~ISubject() = default;
    virtual void attach(IObserver* observer) = 0;
    virtual void detach(IObserver* observer) = 0;
    virtual void notifyObservers() = 0;
};
