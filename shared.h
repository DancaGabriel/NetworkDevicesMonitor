#ifndef SHARED_H
#define SHARED_H

#include <QString>

class SharedData {
public:
    static QString getUsername() { return username; }
    static void setUsername(const QString& newUsername) { username = newUsername; }
    static QString username;

};




#endif // SHARED_H

