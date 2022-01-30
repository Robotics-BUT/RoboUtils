//
// Created by Matous Hybl on 2018-10-14.
//

#pragma once


namespace RoboUtils {

    /// \brief Get curreent msecs
    ///
    /// \return coumt of milliseconds
    long long millis();

    /// \brief Do nothing for specified time
    ///
    /// \param ms count of msecs to do notning
    void delay(long ms);


    /// \brief Check if there was elapsed time from last event
    ///
    /// \param last_event the time of last elapsed event
    /// \param delay time in ms between events
    /// \param ms reference timestamp or -1 to get actual timestamp from system
    /// \return true, if the timeout elapsed (event time is reset]
    bool expired(long long *last_event, long delay, long long ms = -1);
}
