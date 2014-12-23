<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenUserTestCase.
 */

namespace Drupal\token\Tests;

use Drupal\Core\Session\AnonymousUserSession;
use Drupal\field\Entity\FieldStorageConfig;

/**
 * Tests user tokens.
 *
 * @group token
 */
class TokenUserTestCase extends TokenTestBase {
  protected $account = NULL;

  /**
   * Modules to enable.
   *
   * @var array
   */
  protected static $modules = array('token_user_picture');

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp();

    // Enable user pictures.
    \Drupal::state()->set('user_pictures', 1);
    \Drupal::state()->set('user_picture_file_size', '');

    // Set up the pictures directory.
    $picture_path = file_default_scheme() . '://' . \Drupal::state()->get('user_picture_path', 'pictures');
    if (!file_prepare_directory($picture_path, FILE_CREATE_DIRECTORY)) {
      $this->fail('Could not create directory ' . $picture_path . '.');
    }

    $this->account = $this->drupalCreateUser(array('administer users'));
    $this->drupalLogin($this->account);
  }

  function testUserTokens() {
    // Add a user picture to the account.
    $image = current($this->drupalGetTestFiles('image'));
    $edit = array('files[user_picture_0]' => drupal_realpath($image->uri));
    $this->drupalPostForm('user/' . $this->account->id() . '/edit', $edit, t('Save'));

    $storage = \Drupal::entityManager()->getStorage('user');

    // Load actual user data from database.
    $storage->resetCache();
    $this->account = $storage->load($this->account->id());
    $this->assertTrue(!empty($this->account->user_picture->target_id), 'User picture uploaded.');

    $user_picture = array(
      '#type' => 'user_picture',
      '#account' => $this->account,
    );
    $user_tokens = array(
      // ToDo: Use render arrays. See https://drupal.org/node/2195739
      'picture' => drupal_render($user_picture),
      'picture:fid' => $this->account->user_picture->target_id,
      'picture:size-raw' => 125,
      'ip-address' => NULL,
      'roles' => implode(', ', $this->account->getRoles()),
    );
    $this->assertTokens('user', array('user' => $this->account), $user_tokens);

    // Remove the simpletest-created user role.
    $roles = $this->account->getRoles();
    $this->account->removeRole(end($roles));
    $this->account->save();

    // Remove the user picture field and reload the user.
    FieldStorageConfig::loadByName('user', 'user_picture')->delete();
    $storage->resetCache();
    $this->account = $storage->load($this->account->id());

    $user_tokens = array(
      'picture' => NULL,
      'picture:fid' => NULL,
      'ip-address' => NULL,
      'roles' => 'authenticated',
      'roles:keys' => (string) DRUPAL_AUTHENTICATED_RID,
    );
    $this->assertTokens('user', array('user' => $this->account), $user_tokens);

    // The ip address token should work for the current user token type.
    $tokens = array(
      'ip-address' => \Drupal::request()->getClientIp(),
    );
    $this->assertTokens('current-user', array(), $tokens);

    $anonymous = new AnonymousUserSession();
    $tokens = array(
      'roles' => 'anonymous',
      'roles:keys' => (string) DRUPAL_ANONYMOUS_RID,
    );
    $this->assertTokens('user', array('user' => $anonymous), $tokens);
  }
}
