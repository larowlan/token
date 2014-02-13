<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenTaxonomyTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests taxonomy tokens.
 */
class TokenTaxonomyTestCase extends TokenTestBase {
  protected $vocab;

  public static function getInfo() {
    return array(
      'name' => 'Taxonomy and vocabulary token tests',
      'description' => 'Test the taxonomy tokens.',
      'group' => 'Token',
    );
  }

  public function setUp($modules = array()) {
    $modules[] = 'taxonomy';
    parent::setUp($modules);

    // Create the default tags vocabulary.
    $vocabulary = entity_create('taxonomy_vocabulary', array(
      'name' => 'Tags',
      'machine_name' => 'tags',
    ));
    taxonomy_vocabulary_save($vocabulary);
    $this->vocab = $vocabulary;
  }

  /**
   * Test the additional taxonomy term tokens.
   */
  function testTaxonomyTokens() {
    $root_term = $this->addTerm($this->vocab, array('name' => 'Root term', 'path' => array('alias' => 'root-term')));
    $tokens = array(
      'url' => url("taxonomy/term/{$root_term->tid}", array('absolute' => TRUE)),
      'url:absolute' => url("taxonomy/term/{$root_term->tid}", array('absolute' => TRUE)),
      'url:relative' => url("taxonomy/term/{$root_term->tid}", array('absolute' => FALSE)),
      'url:path' => 'root-term',
      'url:unaliased:path' => "taxonomy/term/{$root_term->tid}",
      'edit-url' => url("taxonomy/term/{$root_term->tid}/edit", array('absolute' => TRUE)),
      'parents' => NULL,
      'parents:count' => NULL,
      'parents:keys' => NULL,
      'root' => NULL,
      // Deprecated tokens
      'url:alias' => 'root-term',
    );
    $this->assertTokens('term', array('term' => $root_term), $tokens);

    $parent_term = $this->addTerm($this->vocab, array('name' => 'Parent term', 'parent' => $root_term->tid));
    $tokens = array(
      'url' => url("taxonomy/term/{$parent_term->tid}", array('absolute' => TRUE)),
      'url:absolute' => url("taxonomy/term/{$parent_term->tid}", array('absolute' => TRUE)),
      'url:relative' => url("taxonomy/term/{$parent_term->tid}", array('absolute' => FALSE)),
      'url:path' => "taxonomy/term/{$parent_term->tid}",
      'url:unaliased:path' => "taxonomy/term/{$parent_term->tid}",
      'edit-url' => url("taxonomy/term/{$parent_term->tid}/edit", array('absolute' => TRUE)),
      'parents' => 'Root term',
      'parents:count' => 1,
      'parents:keys' => $root_term->tid,
      'root' => check_plain($root_term->name),
      'root:tid' => $root_term->tid,
      // Deprecated tokens
      'url:alias' => "taxonomy/term/{$parent_term->tid}",
    );
    $this->assertTokens('term', array('term' => $parent_term), $tokens);

    $term = $this->addTerm($this->vocab, array('name' => 'Test term', 'parent' => $parent_term->tid));
    $tokens = array(
      'parents' => 'Root term, Parent term',
      'parents:count' => 2,
      'parents:keys' => implode(', ', array($root_term->tid, $parent_term->tid)),
    );
    $this->assertTokens('term', array('term' => $term), $tokens);
  }

  /**
   * Test the additional vocabulary tokens.
   */
  function testVocabularyTokens() {
    $vocabulary = $this->vocab;
    $tokens = array(
      'machine-name' => 'tags',
      'edit-url' => url("admin/structure/taxonomy/{$vocabulary->machine_name}/edit", array('absolute' => TRUE)),
    );
    $this->assertTokens('vocabulary', array('vocabulary' => $vocabulary), $tokens);
  }

  function addVocabulary(array $vocabulary = array()) {
    $vocabulary += array(
      'name' => drupal_strtolower($this->randomName(5)),
      'nodes' => array('article' => 'article'),
    );
    $vocabulary = entity_create('taxonomy_vocabulary', $vocabulary);
    taxonomy_vocabulary_save($vocabulary);
    return $vocabulary;
  }

  function addTerm(stdClass $vocabulary, array $term = array()) {
    $term += array(
      'name' => drupal_strtolower($this->randomName(5)),
      'vid' => $vocabulary->vid,
    );
    $term = entity_create('taxonomy_term', $term);
    taxonomy_term_save($term);
    return $term;
  }
}
