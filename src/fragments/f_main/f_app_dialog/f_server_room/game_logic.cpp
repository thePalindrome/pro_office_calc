#include "fragments/f_main/f_app_dialog/f_server_room/game_logic.hpp"
#include "fragments/f_main/f_app_dialog/f_server_room/defs.hpp"
#include "raycast/entity_manager.hpp"
#include "raycast/spatial_system.hpp"
#include "raycast/event_handler_system.hpp"
#include "raycast/render_system.hpp"
#include "raycast/damage_system.hpp"
#include "event_system.hpp"
#include "request_state_change_event.hpp"
#include "state_ids.hpp"
#include "utils.hpp"


using std::string;


namespace youve_got_mail {


//===========================================
// GameLogic::GameLogic
//===========================================
GameLogic::GameLogic(EventSystem& eventSystem, EntityManager& entityManager)
  : m_eventSystem(eventSystem),
    m_entityManager(entityManager) {

  DBG_PRINT("GameLogic::GameLogic\n");

  m_eventSystem.listen("youveGotMail/divByZero", [this](const Event& event) {
    onDivByZero(event);
  }, m_eventIdx);

  EventHandlerSystem& eventHandlerSystem =
    m_entityManager.system<EventHandlerSystem>(ComponentKind::C_EVENT_HANDLER);

  SpatialSystem& spatialSystem = m_entityManager.system<SpatialSystem>(ComponentKind::C_SPATIAL);

  const Player& player = *spatialSystem.sg.player;

  entityId_t entityId = Component::getNextId();

  CEventHandler* handlers = new CEventHandler(entityId);
  handlers->broadcastedEventHandlers.push_back(EventHandler{"entity_changed_zone",
    [=, &player](const GameEvent& e_) {

    auto& e = dynamic_cast<const EChangedZone&>(e_);
    if (e.entityId == player.body) {
      if (e.newZone == Component::getIdFromString("level_exit")) {
        m_eventSystem.fire(pEvent_t(new RequestStateChangeEvent(ST_SUICIDE_MISSION, true)));
      }
      else if (e.newZone == Component::getIdFromString("input_sensor_a")
        || e.newZone == Component::getIdFromString("input_sensor_b")) {

        if (m_exitDoorSelectedNum != '\0') {
          if (m_exitDoorInput.length() == 4) {
            m_exitDoorInput = m_exitDoorSelectedNum;
          }
          else {
            m_exitDoorInput += m_exitDoorSelectedNum;
          }

          DBG_PRINT("Current door input: " << m_exitDoorInput.toStdString() << "\n");

          drawExitDoorDigitDisplay();

          if (QString::compare(m_exitDoorInput, exitDoorCode) == 0) {
            DBG_PRINT("Correct door code entered. Opening door.\n");

            EActivateEntity e;
            m_entityManager.fireEvent(e, {Component::getIdFromString("exit_door")});
          }

          m_exitDoorSelectedNum = '\0';
        }
      }
      else if (e.newZone == Component::getIdFromString("zero_sensor")) {
        DBG_PRINT("Selected 0 input\n");
        m_exitDoorSelectedNum = '0';
      }
      else if (e.newZone == Component::getIdFromString("one_sensor")) {
        DBG_PRINT("Selected 1 input\n");
        m_exitDoorSelectedNum = '1';
      }
      else if (e.newZone == Component::getIdFromString("two_sensor")) {
        DBG_PRINT("Selected 2 input\n");
        m_exitDoorSelectedNum = '2';
      }
      else if (e.newZone == Component::getIdFromString("three_sensor")) {
        DBG_PRINT("Selected 3 input\n");
        m_exitDoorSelectedNum = '3';
      }
      else if (e.newZone == Component::getIdFromString("four_sensor")) {
        DBG_PRINT("Selected 4 input\n");
        m_exitDoorSelectedNum = '4';
      }
      else if (e.newZone == Component::getIdFromString("five_sensor")) {
        DBG_PRINT("Selected 5 input\n");
        m_exitDoorSelectedNum = '5';
      }
      else if (e.newZone == Component::getIdFromString("six_sensor")) {
        DBG_PRINT("Selected 6 input\n");
        m_exitDoorSelectedNum = '6';
      }
      else if (e.newZone == Component::getIdFromString("seven_sensor")) {
        DBG_PRINT("Selected 7 input\n");
        m_exitDoorSelectedNum = '7';
      }
      else if (e.newZone == Component::getIdFromString("eight_sensor")) {
        DBG_PRINT("Selected 8 input\n");
        m_exitDoorSelectedNum = '8';
      }
      else if (e.newZone == Component::getIdFromString("nine_sensor")) {
        DBG_PRINT("Selected 9 input\n");
        m_exitDoorSelectedNum = '9';
      }
    }
  }});
  handlers->broadcastedEventHandlers.push_back(EventHandler{"entity_destroyed",
    [this](const GameEvent& e_) {

    auto& e = dynamic_cast<const EEntityDestroyed&>(e_);
    if (e.entityId == Component::getIdFromString("larry")) {
      m_eventSystem.fire(pEvent_t(new Event("youveGotMail/larryKilled")));
    }
  }});
  handlers->broadcastedEventHandlers.push_back(EventHandler{"entity_destroyed",
    [this](const GameEvent& e_) {

    auto& e = dynamic_cast<const EEntityDestroyed&>(e_);
    if (e.entityId == Component::getIdFromString("larry")) {
      m_eventSystem.fire(pEvent_t(new Event("youveGotMail/larryKilled")));
    }
  }});
  int serversDestroyed = 0;
  handlers->broadcastedEventHandlers.push_back(EventHandler{"server_destroyed",
    [this, serversDestroyed](const GameEvent& e) mutable {

    if (++serversDestroyed == 2) {
      m_eventSystem.fire(pEvent_t(new Event("youveGotMail/serversDestroyed")));
    }
  }});

  eventHandlerSystem.addComponent(pComponent_t(handlers));

  drawExitDoorDigitDisplay();
}

//===========================================
// GameLogic::drawExitDoorDigitDisplay
//===========================================
void GameLogic::drawExitDoorDigitDisplay() {
  RenderSystem& renderSystem = m_entityManager.system<RenderSystem>(ComponentKind::C_RENDER);
  Texture& tex = renderSystem.rg.textures.at("number");

  QFont font;
  font.setPixelSize(14);

  QPainter painter(&tex.image);
  painter.setFont(font);
  painter.setPen(QColor(0, 255, 0));

  tex.image.fill(QColor(20, 20, 50));
  painter.drawText(8, 16, m_exitDoorInput);
}

//===========================================
// GameLogic::onDivByZero
//===========================================
void GameLogic::onDivByZero(const Event& event) {
  m_entityManager.broadcastEvent(GameEvent("div_by_zero"));
}

//===========================================
// GameLogic::~GameLogic
//===========================================
GameLogic::~GameLogic() {
  m_eventSystem.forget(m_eventIdx);
}


}
