<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenUserTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests user tokens.
 */
class TokenUserTestCase extends TokenTestBase {
  protected $account = NULL;

  public static function getInfo() {
    return array(
      'name' => 'User token tests',
      'description' => 'Test the user tokens.',
      'group' => 'Token',
    );
  }

  public function setUp($modules = array()) {
    parent::setUp($modules);

    // Enable user pictures.
    variable_set('user_pictures', 1);
    variable_set('user_picture_file_size', '');

    // Set up the pictures directory.
    $picture_path = file_default_scheme() . '://' . variable_get('user_picture_path', 'pictures');
    if (!file_prepare_directory($picture_path, FILE_CREATE_DIRECTORY)) {
      $this->fail('Could not create directory ' . $picture_path . '.');
    }

    $this->account = $this->drupalCreateUser(array('administer users'));
    $this->drupalLogin($this->account);
  }

  function testUserTokens() {
    // Add a user picture to the account.
    $image = current($this->drupalGetTestFiles('image'));
    $edit = array('files[picture_upload]' => drupal_realpath($image->uri));
    $this->drupalPost('user/' . $this->account->uid . '/edit', $edit, t('Save'));

    // Load actual user data from database.
    $this->account = user_load($this->account->uid, TRUE);
    $this->assertTrue(!empty($this->account->picture->fid), 'User picture uploaded.');

    $user_tokens = array(
      'picture' => theme('user_picture', array('account' => $this->account)),
      'picture:fid' => $this->account->picture->fid,
      'picture:size-raw' => 125,
      'ip-address' => NULL,
      'roles' => implode(', ', $this->account->roles),
      'roles:keys' => implode(', ', array_keys($this->account->roles)),
    );
    $this->assertTokens('user', array('user' => $this->account), $user_tokens);

    $edit = array('user_pictures' => FALSE);
    $this->drupalPost('admin/config/people/accounts', $edit, 'Save configuration');
    $this->assertText('The configuration options have been saved.');

    // Remove the simpletest-created user role.
    user_role_delete(end($this->account->roles));
    $this->account = user_load($this->account->uid, TRUE);

    $user_tokens = array(
      'picture' => NULL,
      'picture:fid' => NULL,
      'ip-address' => NULL,
      'roles' => 'authenticated user',
      'roles:keys' => (string) DRUPAL_AUTHENTICATED_RID,
    );
    $this->assertTokens('user', array('user' => $this->account), $user_tokens);

    // The ip address token should work for the current user token type.
    $tokens = array(
      'ip-address' => ip_address(),
    );
    $this->assertTokens('current-user', array(), $tokens);

    $anonymous = drupal_anonymous_user();
    // Mess with the role array to ensure we still get consistent output.
    $anonymous->roles[DRUPAL_ANONYMOUS_RID] = DRUPAL_ANONYMOUS_RID;
    $tokens = array(
      'roles' => 'anonymous user',
      'roles:keys' => (string) DRUPAL_ANONYMOUS_RID,
    );
    $this->assertTokens('user', array('user' => $anonymous), $tokens);
  }
}
