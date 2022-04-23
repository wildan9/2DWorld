/* by Wildan R. */

#include "Game.h"

Game::Game()
{
	InitWindow(m_window_width, m_window_height, m_window_title.c_str());
	InitAudioDevice();
	SetTargetFPS(60);
}

Game::~Game()
{
	CloseWindow();
	CloseAudioDevice();
}

void Game::Run()
{
	Player wildan{ Vector2{40.0f, 140.0f} };

	Animals animals;

	Map map;

	animals.SetBatFlyRadius(map.GetDreamlandSize().width * map.GetMapScale());

	Prop props;

	m_camera.target = wildan.GetPosition();
	m_camera.offset = Vector2{ m_window_width / 2.0f,  m_window_height / 2.0f };
	m_camera.rotation = 0.0f;
	m_camera.zoom = 1.0f;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(SKYBLUE);
		
		m_camera.target = wildan.GetPosition();
		m_camera.BeginMode();

		map.Draw();

		if (wildan.GetPosition().x < -15.0f || wildan.GetPosition().y < 0.0f || 
			wildan.GetPosition().x > ((float)map.GetDreamlandSize().width - 15.0f) * map.GetMapScale() &&
			wildan.GetPosition().x < (float)map.GetDreamlandSize().width + 1500.0f ||
			wildan.GetPosition().y > (float)map.GetDreamlandSize().height * map.GetMapScale()) wildan.OnWater();
		else wildan.OnLand();

		wildan.Draw();

		animals.Draw(GetFrameTime());

		props.Draw();

		if (CheckCollisionRecs(wildan.GetCollision(), animals.crocodile.GetCollision()) &&
			wildan.IsPunch() && (wildan.GetFacing() == 1.0f && animals.crocodile.GetFacing() >= -1.0f &&
			wildan.GetPosition().x < animals.crocodile.GetPosition().x)) animals.crocodile.Hurt();
		else if (CheckCollisionRecs(wildan.GetCollision(), animals.crocodile.GetCollision()) &&
			wildan.IsPunch() && (wildan.GetFacing() == -1.0f && animals.crocodile.GetFacing() >= -1.0f &&
			wildan.GetPosition().x > animals.crocodile.GetPosition().x)) animals.crocodile.Hurt();
		else animals.crocodile.Walk();

		if (CheckCollisionRecs(wildan.GetCollision(), animals.rhino1.GetRectColl()) ||
			CheckCollisionRecs(wildan.GetCollision(), animals.rhino2.GetRectColl()) ||
			CheckCollisionRecs(wildan.GetCollision(), animals.rhino3.GetRectColl())) wildan.Stop();

		if (CheckCollisionRecs(wildan.GetCollision(), map.GetMapLine1()) ||
			CheckCollisionRecs(wildan.GetCollision(), map.GetMapLine2())) wildan.Stop();

		if (CheckCollisionRecs(wildan.GetCollision(), props.invisible_fence.GetFenceRectangle1()) ||
			CheckCollisionRecs(wildan.GetCollision(), props.invisible_fence.GetFenceRectangle2())) wildan.Stop();

		if (CheckCollisionRecs(wildan.GetCollision(), props.invisible_fence.GetTreeRectangle1()) ||
			CheckCollisionRecs(wildan.GetCollision(), props.invisible_fence.GetTreeRectangle2())) wildan.Stop();

		if (CheckCollisionRecs(wildan.GetCollision(), props.natural_obj.GetBigStone1Coll())) wildan.SetPosition(Vector2{ 3000.0f, 300.0f });

		if (CheckCollisionRecs(wildan.GetCollision(), props.natural_obj.GetBigStone2Coll())) wildan.SetPosition(Vector2{ 40.0f, 140.0f });

		m_camera.EndMode();
		EndDrawing();
	}
}