#ifndef MOTION_H
#define MOTION_H

class Motion {  
    private:
        // Physics attributes
        float _t, _d, _v, _a;
    public:
        /* Getters/Setters */
        float getTime() const;
        void setTime(float t);
        float getDisp() const;
        void setDisp(float d);
        float getVel() const;
        void setVel(float v);
        float getAccel() const;
        void setAccel(float a);

        /* Ctor */
        Motion();

        /* Dtor */
        ~Motion();
};

#endif /* MOTION_H */
