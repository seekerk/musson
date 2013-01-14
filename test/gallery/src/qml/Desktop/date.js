function monthString(month) {
    switch (month) {
    case 0:
        return qsTr("January")
    case 1:
        return qsTr("Februry")
    case 2:
        return qsTr("March")
    case 3:
        return qsTr("April")
    case 4:
        return qsTr("May")
    case 5:
        return qsTr("June")
    case 6:
        return qsTr("July")
    case 7:
        return qsTr("August")
    case 8:
        return qsTr("September")
    case 9:
        return qsTr("October")
    case 10:
        return qsTr("November")
    case 11:
        return qsTr("December")
    }
}

function formattedTime(time) {
    // parse "dd.MM.yyyy hh:mm:ss"
    var day = time.substring(0, 2)
    var month = time.substring(3, 5)
    var year = time.substring(6, 10)
    var hours = time.substring(11, 13)
    var minutes = time.substring(14, 16)
    var seconds = time.substring(17, 19)

    var date = new Date(year, month - 1, day, hours, minutes, seconds)
    var curDate = new Date()

    if ((curDate.getFullYear() == date.getFullYear()) && (curDate.getMonth() == date.getMonth())
            && ((curDate.getDate() - date.getDate()) <= 1)) {
        if ((curDate.getDate() - date.getDate()) == 1)
            return date.getHours() + ":" + date.getMinutes() + ", " + qsTr("Yesterday")

        var hoursAgo = curDate.getHours() - date.getHours();
        if (hoursAgo <= 1) {
            var minutesAgo = curDate.getMinutes() - date.getMinutes() + hoursAgo * 60
            if (minutesAgo < 60) {
                if (minutesAgo <= 1) {
                    var secondsAgo = curDate.getSeconds() - date.getSeconds() + minutesAgo * 60
                    if (secondsAgo < 60) {
                        if (secondsAgo == 1)
                            return qsTr("An 1 second ago")
                        else
                            return secondsAgo + qsTr(" seconds ago")
                    } else {
                        return qsTr("An 1 minute ago")
                    }
                }
                return minutesAgo + qsTr(" minutes ago")
            } else {
                if (minutesAgo == 0) {
                    return qsTr("An 1 hour ago")
                }
            }
        }
        if (date.getMinutes() / 10 < 1)
            return date.getHours() + ":" + "0" + date.getMinutes()
        else
            return date.getHours() + ":" + date.getMinutes()
    } else {
        if (curDate.getFullYear() == date.getFullYear()) {
            if (date.getMinutes() / 10 < 1)
                return date.getHours() + ":" + "0" + date.getMinutes() + ", " + monthString(date.getMonth()) + " " + date.getDate()
            else
                return date.getHours() + ":" + date.getMinutes() + ", " + monthString(date.getMonth()) + " " + date.getDate()
        }
    }
    if (date.getMinutes() / 10 < 1)
        return date.getHours() + ":" + "0" + date.getMinutes() + ", " + monthString(date.getMonth()) + " " + date.getDate() + ", " + date.getFullYear()
    else
        return date.getHours() + ":" + date.getMinutes() + ", " + monthString(date.getMonth()) + " " + date.getDate() + ", " + date.getFullYear()
}
