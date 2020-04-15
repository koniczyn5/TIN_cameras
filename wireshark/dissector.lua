-- Moduł do wireshark'a
-- Autor: Adam Bieniek

cameralink_proto = Proto("cameralink","CameraLink Protocol")
-- create a function to dissect it
function cameralink_proto.dissector(buffer,pinfo,tree)
    pinfo.cols.protocol = "CAMERALINK"
    local subtree = tree:add(cameralink_proto,buffer(),"CameraLink Protocol Data")
    local msg_type = buffer(0,1):bitfield(0, 3)
    
    if msg_type == 1 then 
        subtree:add(buffer(0,1),"TEST_MSG")
        pinfo.cols.info:append(" TEST_MSG")
        local timeout = buffer(0,4):bitfield(3, 14)
        local period = buffer(0,4):bitfield(17, 14)

        subtree:add(buffer(0,1),"timeout: " .. timeout)
        subtree:add(buffer(0,1),"period: " .. period)

    elseif msg_type == 2 then 
        subtree:add(buffer(0,1),"TEST_ACK")
        pinfo.cols.info:append(" TEST_ACK")
        local timeout = buffer(0,4):bitfield(3, 14)
        local period = buffer(0,4):bitfield(17, 14)

        subtree:add(buffer(0,1),"timeout: " .. timeout)
        subtree:add(buffer(0,1),"period: " .. period)
    else
        subtree:add(buffer(0,1),"UNKNOWN_MSG " .. msg_type)
        pinfo.cols.info:append(" UNKNOWN_MSG")
    end
end
-- load the udp.port table
udp_table = DissectorTable.get("udp.port")
-- register our protocol to handle udp ports 4000-5000
for i=4000,5000,1 do
    udp_table:add(i,cameralink_proto)
end