#ifndef DATA_H_
#define DATA_H_

class Data {
public:
	Data();
	~Data();
	
	void getTT();
	void setTT();
	void getTF();
	void setTF();
	void getTR();
	void setTR();
	void getEndVelocity();
	void setDndVelocity();
	void getDirection();
	void setDirection();
private:
	float tt;
	float tf;
	float tr;
	float endVelocity;
	bool direction;
};
#endif // Data_H_
