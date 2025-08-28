#include "pch.h"
#include "Scene.h"
#include "glm.hpp"
// Temporary
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

NLOHMANN_JSON_SERIALIZE_ENUM(AssetType, {
	{AssetType::GameObject, "GameObject"},
	{AssetType::LightObject, "LightObject"}
	})

Scene::Scene() :
	m_SceneID("000DEFAULT"), 
	m_FilePath("defaultScene.json"), 
	m_Shader("src/Graphics/vertex_shader.vert", "src/Graphics/fragment_shader.frag")
{
}

Scene::Scene(const std::string_view &id) : m_SceneID(id)
{
	// Map the SceneId to an actual scene filepath here, or do it at the Load function
}

void Scene::Load()
{
	//m_HandGun = Model("resources/models/handgun/1911.fbx");
	//m_Bagpack = Model("resources/models/explorerBag/backpack.obj");
	//m_WoodenBox = Model("resources/models/woodenBoxes/box.fbx");
	//m_Mp7 = Model("resources/models/mp7/mp7.fbx");
	
	// 1. Open the Scene file path and check for errors
	try {
		std::string relPathFromProjDir = "src/Scene/" + m_FilePath;
		std::ifstream fileStream(relPathFromProjDir);
		if (!fileStream.is_open()) {
			LOG_ERROR("Cannot open the input file stream of the Scene::Load() : {}", m_FilePath);
			return;
		}
		json Jdata = json::parse(fileStream);
		if (std::string SceneId = Jdata.at("scene_id"); SceneId.compare(m_SceneID) != 0) {
			LOG_ERROR("Error from Json {}: 'scene_id' inside the json does not match the 'scene_id' of the Scene instance", m_FilePath);
			return;
		}
		// Get the assets objects
		json JAssets = Jdata.at("assets");
		if (JAssets.empty()) {
			LOG_ERROR("Error reading Json {}: assets is missing or empty", m_FilePath);
			return;
		}
		// 2. Obtain every asset data and load in to the assets container
		for (auto it = JAssets.begin(); it != JAssets.end(); ++it)
		{
			json JAssetObject = (*it);
			AssetType type = JAssetObject.at("type");
			if (type == AssetType::GameObject)
			{
				AssetData assetData;
				assetData.type = type;
				assetData.filePath = JAssetObject.at("model_file_path");
				json pos = JAssetObject.at("position");
				assetData.position = glm::vec3(pos[0], pos[1], pos[2]);
				json rot = JAssetObject.at("rotation");
				assetData.rotation = glm::vec3(rot[0], rot[1], rot[2]);
				assetData.scale = JAssetObject.at("scale");
				assetData.colissions = JAssetObject.at("colissions");

				m_AssetContainer.push_back(new Asset(assetData));
			}
		}
	}
	catch (const nlohmann::json::parse_error& e) {
		LOG_FATAL("Json Parse Error[{}]: {}", e.id, e.what());
		throw;
	}
}

void Scene::Render(Camera& camera)
{
	m_Shader.use();

	// Light source
	// Point of light
	glm::vec3 lightSourcePosition(0.0f, 10.0f, 0.0f);

	glm::mat4 lightTransform(1.0f);
	glm::mat4 translate = glm::translate(lightTransform, glm::vec3(0.0f, 0.0f, 5.0f));
	glm::mat4 rotate = glm::rotate(lightTransform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

	lightTransform = rotate * translate;

	glm::vec4 lightPosV4(lightSourcePosition, 1.0f);	// tranform to vec4
	lightPosV4 = lightTransform * lightPosV4; // apply transformation
	lightSourcePosition = lightPosV4;	//transform back to vec3

	m_Shader.setVec3("light.position", lightSourcePosition);

	// Directional light
	glm::vec3 lightDirection(-0.2f, -1.0f, -0.3f);
	m_Shader.setVec3("light.direction", lightDirection);

	// light color and intensity/strenght
	glm::vec3 lightAmbient(0.2f);
	glm::vec3 lightDiffuse(0.8f);
	glm::vec3 lightSpecular(1.0f);
	m_Shader.setVec3("light.ambient", lightAmbient);
	m_Shader.setVec3("light.diffuse", lightDiffuse);
	m_Shader.setVec3("light.specular", lightSpecular);
	m_Shader.setVec3("viewPos", camera.getPosition());

	// Only renders assets of type GameObject here
	for (Asset* asset : m_AssetContainer)
	{
		AssetData& assetData = asset->GetData();

		if (assetData.type == AssetType::GameObject) {
			WorldTransform worldTransform;

			worldTransform.setTranslation(assetData.position);
			worldTransform.setScale(assetData.scale);
			if (assetData.rotation != glm::vec3{ 0.0f, 0.0f, 0.0f }) {	// rotation exists
				float degrees = 0.0f;
				glm::vec3 rotationAxis{ 0.0f };
				if (assetData.rotation[0] != 0.0f) {
					degrees = assetData.rotation[0];
					rotationAxis[0] = 1.0f;
				} else if (assetData.rotation[1] != 0.0f) {
					degrees = assetData.rotation[1];
					rotationAxis[1] = 1.0f;
				} else if (assetData.rotation[2] != 0.0f) {
					degrees = assetData.rotation[2];
					rotationAxis[2] = 1.0f;
				}
				worldTransform.setRotation(glm::radians(degrees), rotationAxis);
			}

			glm::mat4 worldTransformMatrix = worldTransform.getMatrix();
			glm::mat4 cameraView = camera.getView();
			glm::mat4 projectionTransformMatrix = m_projectionTransform.getMatrix();

			m_Shader.setMatrix4("model", worldTransformMatrix);
			m_Shader.setMatrix4("view", cameraView);
			m_Shader.setMatrix4("projection", projectionTransformMatrix);

			asset->Draw(m_Shader);

		}
	}
}
