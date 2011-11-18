/**
 * @file
 * Main JavaScript library for our office hours JavaScript.
 */

// Object for holding our functions.
Drupal.officeHours = {
  timeMatcher: /([12]?\d)[:.]?(\d{2})/,
  twoDigitMatcher: /([12]\d|[1-9])/,
  yearWeekMatcher: /(\d{4})W?(\d{2})/,
  nidMatcher: /node-(\d+)/
};

