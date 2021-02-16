using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyBullet : MonoBehaviour
{
    public float speed = 5;
	public float lifespan = 3;
    float lifespanRemain;

    MeshRenderer modelRenderer;

    void Start() {
		Destroy(gameObject, lifespan);
        lifespanRemain = lifespan;

        var model = transform.Find("Model");
        if (model) {
            modelRenderer = model.GetComponent<MeshRenderer>();
        }
    }

	void Update()
    {
        lifespanRemain -= Time.deltaTime;
        if (lifespanRemain < 0) lifespanRemain = 0;

        if (modelRenderer) {
            modelRenderer.material.color = Color.Lerp(new Color(0,1,0), new Color(1,0,0), lifespanRemain / lifespan);
        }

        transform.Translate(0, 0, speed * Time.deltaTime);
    }

	private void OnTriggerEnter(Collider other)
	{
		Debug.Log("enter");
	}
}
