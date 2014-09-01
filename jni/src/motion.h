#ifndef MOTION_H
#define MOTION_H

class Motion {  
    private:
        // Physics attributes
        float _ct, _t, _d, _v, _a;
    public:
        /* Getters/Setters */
        float getChangeTime();
        void setChangeTime(const float t);
        float getTime();
        void setTime(const float t);
        float getDisp();
        void setDisp(const float d);
        float getVel();
        void setVel(const float v);
        float getAccel();
        void setAccel(const float a);

        /* Ctor */
        Motion();

        /* Dtor */
        ~Motion();
};

#endif /* MOTION_H */
