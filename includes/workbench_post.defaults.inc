<?php
/**
 * @file
 * Default text for email subjects lines and bodies
 */

define('DEFAULT_BODY_REVISION_AUTHOR_NEEDS_REVIEW_TO_PUBLISHED',
'Hello

The page you submitted for review, [node:title] ([node:url]), has been approved and
published.

Regards,

[site:name]');

define('DEFAULT_BODY_REVISION_AUTHOR_NEEDS_REVIEW_TO_DRAFT',
'Hello

The page you submitted for review, [node:title] ([node:url]), has been returned to draft. 

Please check your content conforms to style guidelines and contact [site:mail] for more information.

Regards,

[site:name]');

define('DEFAULT_BODY_REVISION_AUTHOR_DRAFT_TO_NEEDS_REVIEW',
'Hello

The page you edited, [node:title] ([node:url]), has been successfully submitted for review. 

We aim to review your content soon. For more information please contact [site:mail].

Regards,

[site:name]');

define('DEFAULT_BODY_REVISION_AUTHOR_PUBLISHED_TO_NEEDS_REVIEW',
    'Hello

The page you edited, [node:title] ([node:url]), has been successfully submitted for review. 

We aim to review your content soon. For more information please contact [site:mail].

Regards,

[site:name]');

define('DEFAULT_BODY_NEEDS_REVIEW_TO_PUBLISHED',
'Hello

A revision to page [node:title] ([node:url]) has been approved and
published.

Regards,

[site:name]');

define('DEFAULT_BODY_NEEDS_REVIEW_TO_DRAFT',
'Hello

A revision to page [node:title] ([node:url]) has been returned to draft. 

Please contact [site:mail] for more information.

Regards,

[site:name]');

define('DEFAULT_BODY_DRAFT_TO_NEEDS_REVIEW',
'Hello

Page [node:title] ([node:url]) has been edited and submitted for review.

Regards,

[site:name]');

define('DEFAULT_BODY_PUBLISHED_TO_NEEDS_REVIEW',
    'Hello

Page [node:title] ([node:url]) has been edited and submitted for review.

Regards,

[site:name]');

define('DEFAULT_SUBJECT_NEEDS_REVIEW_TO_PUBLISHED',

  'Page [node:title] has been approved and published');

define('DEFAULT_SUBJECT_NEEDS_REVIEW_TO_DRAFT',

  'Page [node:title] has been returned to draft mode');

define('DEFAULT_SUBJECT_DRAFT_TO_NEEDS_REVIEW',

  'Page [node:title] has been submitted for editorial review');

define('DEFAULT_SUBJECT_PUBLISHED_TO_NEEDS_REVIEW',

  'Page [node:title] has been submitted for editorial review');
