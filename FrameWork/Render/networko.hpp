#include <FrameWork/Utilities/Memory.hpp>
class network{
public:
    uint8_t get_owner_id(uint64_t net_object) {
        return FrameWork::Memory::ReadMemory<uint8_t>(net_object + 0x49);
    }

    bool set_network_id_can_migrate(uint64_t net_object, bool toggle) {
        {
            auto word_004E = FrameWork::Memory::ReadMemory<uint16_t>(net_object + 0x4E);
            if (!toggle)
                FrameWork::Memory::wrm<uint16_t>(net_object + 0x4E, word_004E | 1);
            else
                FrameWork::Memory::wrm<uint16_t>(net_object + 0x4E, word_004E & ~1);
        }
        return true;
    }

    bool set_network_id_exists_on_all_machines(uint64_t net_object, bool toggle) {
        auto word_004E = FrameWork::Memory::ReadMemory<uint16_t>(net_object + 0x4E);
        if (toggle)
            FrameWork::Memory::wrm<uint16_t>(net_object + 0x4E, word_004E | 8);
        else
            FrameWork::Memory::wrm<uint16_t>(net_object + 0x4E, word_004E & ~8);
        return true;
    }

    bool network_request_control_of_network_id(uint64_t net_object, uint64_t local_net_object) {
        FrameWork::Memory::wrm<uint8_t>(net_object + 0x49, get_owner_id(local_net_object));
        FrameWork::Memory::wrm<uint8_t>(net_object + 0x4A, 0xFF);
        FrameWork::Memory::wrm<uint8_t>(net_object + 0x4B, 0);
        return true;
    }

    void network_request_control_of_entity(uint64_t entity, uint64_t localplayer) {
        uint64_t entity_net_object = FrameWork::Memory::ReadMemory<uint64_t>(entity + 0xD0);
        uint64_t local_net_object = FrameWork::Memory::ReadMemory<uint64_t>(localplayer + 0xD0);
        set_network_id_can_migrate(entity_net_object, true);
        set_network_id_exists_on_all_machines(entity_net_object, true);
        network_request_control_of_network_id(entity_net_object, local_net_object);
    }
};
