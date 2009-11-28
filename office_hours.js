// $Id$

/**
 * @file
 * Utility functions for our office hours JavaScript.
 */


// Empty object for holding our functions.
Drupal.officeHours = {
  timeMatcher: /(\d{1,2})[:.]?(\d{2})/,
  twoDigitMatcher: /(\d{1,2})/,
  yearWeekMatcher: /(\d{4})W?(\d{2})/,
  sanitiseTimeInput: function (elem) {
    var $this = $(elem);
    var newVal = ''; 

    // Try matching for full time.
    var match = $this.val().match(Drupal.officeHours.timeMatcher);
    if (match) {
      // match[1] is hours, match[2] is minutes.
      // Prefix zero to single-digit hours. 
      if (match[1] < 10) {
        newVal += '0' + parseInt(match[1]);
      }
      else if (match[1] < 24) {
        newVal += match[1];
      }
      // If set to more than 23, go back down.
      else {
        newVal += 23;
      }

      if (match[2] < 60) {
        newVal += ':' + match[2];
      }
      else {
        newVal += ':59';
      }
    }
    else {
      // Try matching just an hours value.
      var match = $this.val().match(Drupal.officeHours.twoDigitMatcher);
      if (match) {
        // Prefix zero to single-digit hours. 
        if (match[1] < 10) {
          newVal += '0' + parseInt(match[1]);
        }
        else if (match[1] < 24) {
          newVal += match[1];
        }
        // If set to more than 23, go back down.
        else {
          newVal += 23;
        }

        newVal += ':00';
      }
    }
    
    $this.val(newVal);
  },
  sanitiseWeekInput: function (elem) {
    var $this = $(elem);
    var newVal = '';

    var match = $this.val().match(Drupal.officeHours.yearWeekMatcher);
    if (match) {
      newVal += parseInt(match[1]);
      newVal += 'W';

      // Prefix zero to single-digit weeks.
      if (match[2] < 10) {
        newVal += '0' + parseInt(match[2]);
      }
      else if (match[2] < 54) {
        newVal += match[2];
      }
      else {
        newVal += '01';
      }
    }
    else {
      // If we couldn't match a full value, try finding a week number.
      var match = $this.val().match(Drupal.officeHours.twoDigitMatcher);
      if (match && match[1] < 54) {
        var year = new Date().getFullYear();
        newVal += year + 'W' + match[1];
      }
    }

    $this.val(newVal);
  }
};

