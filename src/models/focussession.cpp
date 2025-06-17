#include "focussession.h"

FocusSession::FocusSession(const QDateTime &start, const QDateTime &end, int durationMinutes)
    : m_startTime(start), m_endTime(end), m_durationMinutes(durationMinutes)
{}

QDateTime FocusSession::startTime() const { return m_startTime; }
QDateTime FocusSession::endTime() const { return m_endTime; }
int FocusSession::duration() const { return m_durationMinutes; }
