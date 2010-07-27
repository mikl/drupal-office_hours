<?php
// $Id$

/**
 * @file
 * Formatter for outputting office hours for a single day.
 */

foreach ($day as $hours):
  if (!empty($hours)):
?>

<span class="hours">
  <span class="open"><?php print office_hours_format_time($hours['open']); ?></span> –
  <span class="close"><?php print office_hours_format_time($hours['close']); ?></span>
</span>

<?php
  else: 
?>

<span class="hours closed"><?php print t('closed'); ?></span>';

<?php
  endif;
endforeach;
?>

