using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Lesson009_Portal : MonoBehaviour
{
	public string destLevel;
	public bool loadAsync = false;

	private void OnDrawGizmos()
	{
		var box = GetComponent<BoxCollider>();
		if (box) {
			Gizmos.matrix = transform.localToWorldMatrix;
			Gizmos.color = new Color(0,0,1, 0.5f);
			Gizmos.DrawCube(box.center, box.size);
		}
	}

    public void LoadLevel() {
        Debug.Log($"Load Scene {destLevel}");

		if (loadAsync) {
			StartCoroutine(LoadLevelAsync(destLevel));
			return;
		}

		UnityEngine.SceneManagement.SceneManager.LoadScene(destLevel);
	    Debug.Log($"Scene {destLevel} Loaded");
    }

    IEnumerator LoadLevelAsync(string level) {
        AsyncOperation asyncLoad = UnityEngine.SceneManagement.SceneManager.LoadSceneAsync(destLevel);
        while (!asyncLoad.isDone)
        {
            yield return null;
        }
        Debug.Log($"Scene {destLevel} Loaded");
    }

}
