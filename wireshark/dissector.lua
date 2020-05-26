-- Moduł do wireshark'a
-- Projekt: Camera-Link
-- Autor: Adam Bieniek
-- Data utworzenia: 15.04.2020

cameralink_proto = Proto("cameralink","CameraLink Protocol")
-- create a function to dissect it
function cameralink_proto.dissector(buffer,pinfo,tree)
    pinfo.cols.protocol = "CAMERALINK"
    local subtree = tree:add(cameralink_proto,buffer(),"CameraLink Protocol Data")
    local msg_type = buffer(0,1):uint()
    
    if msg_type == 1 then 
        subtree:add(buffer(0,1),"TEST_REQ")
        pinfo.cols.info:append(" TEST_REQ")
    elseif msg_type == 2 then 
        subtree:add(buffer(0,1),"TEST_ACK")
        pinfo.cols.info:append(" TEST_ACK")
        local testresult = buffer(1,1):uint()
	subtree:add(buffer(0,1),"Auto test result: " .. testresult)
    elseif msg_type == 3 then
	subtree:add(buffer(0,1),"CONF_REQ")
        pinfo.cols.info:append(" CONF_REQ")
	local rHor = buffer(1,4):le_uint()
	local rVer = buffer(5,4):le_uint()
	subtree:add(buffer(0,1),"Resolution: ".. rHor .. "x" .. rVer)
	local focalLength = buffer(9,4):le_float()
	subtree:add(buffer(0,1),"focalLength: ".. focalLength)
	local interval = buffer(13,4):le_float()
	subtree:add(buffer(0,1),"Interval: ".. interval)
	local jitter = buffer(17,4):le_float()
	subtree:add(buffer(0,1),"Jitter: ".. jitter)
    elseif msg_type == 4 then
        subtree:add(buffer(0,1),"CONF_ACK")
        pinfo.cols.info:append(" CONF_ACK")
        local testresult = buffer(1,1):uint()
	subtree:add(buffer(0,1),"Auto test result: " .. testresult)
    elseif msg_type == 5 then
	subtree:add(buffer(0,1),"INST_REQ")
        pinfo.cols.info:append("INST_REQ")
	subtree:add(buffer(0,1),"Password: " .. buffer:raw(1,4))
    elseif msg_type == 6 then
	subtree:add(buffer(0,1),"INST_ACK")
        pinfo.cols.info:append("INST_ACK")
    elseif msg_type == 7 then
	subtree:add(buffer(0,1),"DISC_REQ")
        pinfo.cols.info:append("DISC_REQ")
    elseif msg_type == 8 then
	subtree:add(buffer(0,1),"DISC_ACK")
        pinfo.cols.info:append("DISC_ACK")
    elseif msg_type == 9 then
	subtree:add(buffer(0,1),"DATA_MSG")
        pinfo.cols.info:append("DATA_MSG")
	local packet = buffer(1,4):le_uint()
	local maxPackets = buffer(5,4):le_uint()
	subtree:add(buffer(0,1),"Packet: " .. packet .."/" .. maxPackets)
    elseif msg_type == 10 then
	subtree:add(buffer(0,1),"DATA_RQT")
        pinfo.cols.info:append("DATA_RQT")
	local requestedPacket = buffer(1,4):le_uint()
	subtree:add(buffer(0,1),"Requested packet: " .. requestedPacket)
    elseif msg_type == 11 then
	subtree:add(buffer(0,1),"DATA_ACK")
        pinfo.cols.info:append("DATA_ACK")
    elseif msg_type == 12 then
	subtree:add(buffer(0,1),"DATA_HDR")
        pinfo.cols.info:append("DATA_HDR")
	local filenameSize = buffer(1,4):le_uint()
	local maxPackets = buffer(5,4):le_uint()
	subtree:add(buffer(0,1),"Number of packets: " .. maxPackets ..". Filename: " .. buffer:raw(9,filenameSize))
    elseif msg_type == 13 then
	subtree:add(buffer(0,1),"NO_PAIR")
        pinfo.cols.info:append("NO_PAIR")
    elseif msg_type == 14 then
	subtree:add(buffer(0,1),"IS_PAIR")
        pinfo.cols.info:append("IS_PAIR")
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
