#include <iostream>
 
 
using namespace std;
 
 
class ObjectMemento {
 
    private:
        static const unsigned arraySize = 2;
        unsigned context[arraySize];
    public:
        ObjectMemento();
        void saveMember(const unsigned index,const unsigned member);
        unsigned fechtMember(const unsigned index);
};
 
 
ObjectMemento::ObjectMemento() {
    this->context[0] = 0;
    this->context[1] = 0;
}
 
void ObjectMemento::saveMember(const unsigned index, const unsigned member){
    this->context[index] = member;
}
 
unsigned ObjectMemento::fechtMember(const unsigned index){
        return this->context[index];
}
 
class Object {
 
    private:
        unsigned memberA;
        unsigned memberB;
 
    public:
        /*Here We declared our getters and setters*/
        void setMemberA(const unsigned valueMember);
        void setMemberB(const unsigned valueMember);
        unsigned getMemberA();
        unsigned getMemberB();
 
        Object(unsigned memberA, unsigned memberB);
        void createMemento(ObjectMemento **outMem); 
        void restoreMemento(ObjectMemento * const inMem);
};
 
Object::Object(unsigned memberA, unsigned memberB) : memberA(memberA) , memberB(memberB){
     
}
 
unsigned Object::getMemberA(){ return this->memberA; }
 
unsigned Object::getMemberB(){ return this->memberB; }
 
void Object::setMemberA(const unsigned valueMember){
    this->memberA = valueMember;
}
 
void Object::setMemberB(const unsigned valueMember){
    this->memberB = valueMember;
}
 
 
 
void Object::createMemento(ObjectMemento **outMem){
 
    ObjectMemento *p = *outMem;
    p->saveMember( 0 , this->getMemberA() );
    p->saveMember( 1 , this->getMemberB() );
 
}
 
void Object::restoreMemento(ObjectMemento * const inMem){
 
    this->setMemberA( inMem->fechtMember( 0 ) );
    this->setMemberB( inMem->fechtMember( 1 ) );
 
}
 
 
 
int main(){
 
    Object *obj = 0;
    ObjectMemento *snapshot = 0;
 
    obj = new Object(100,50);
    snapshot = new ObjectMemento;
 
    cout << "Before snapshot" << endl;
    cout << obj->getMemberA() << endl;
    cout << obj->getMemberB() << endl;
 
    obj->createMemento(&snapshot);
 
    obj->setMemberA(7);
    obj->setMemberB(45);
     
    cout << "===============================================" << endl;
    cout << "It is after we change the member values!" << endl;
    cout << obj->getMemberA() << endl;
    cout << obj->getMemberB() << endl;
 
    obj->restoreMemento(snapshot);
 
    delete snapshot;
 
    cout << "===============================================" << endl;
    cout << "It is after we have restored the latest snapshot" << endl;
    cout << obj->getMemberA() << endl;
    cout << obj->getMemberB() << endl;
 
        delete obj;
 
    return 0;
 
}
