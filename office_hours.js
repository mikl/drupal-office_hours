// $Id$

/**
 * @file
 * Main JavaScript library for our office hours JavaScript.
 */

// Object for holding our functions.
Drupal.officeHours = {
  timeMatcher: /(\d{1,2})[:.]?(\d{2})/,
  twoDigitMatcher: /(\d{1,2})/,
  yearWeekMatcher: /(\d{4})W?(\d{2})/,
  nidMatcher: /node-(\d+)/
};

