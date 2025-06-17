#ifndef FOCUSSESSION_H
#define FOCUSSESSION_H

#include <QDateTime>

class FocusSession
{
public:
    FocusSession(const QDateTime &start, const QDateTime &end, int durationMinutes);

    QDateTime startTime() const;
    QDateTime endTime() const;
    int duration() const;

private:
    QDateTime m_startTime;
    QDateTime m_endTime;
    int m_durationMinutes;
};

#endif // FOCUSSESSION_H
