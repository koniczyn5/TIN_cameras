-- Moduł do wireshark'a
-- Autor: Adam Bieniek

cameralink_proto = Proto("cameralink","CameraLink Protocol")
-- create a function to dissect it
function cameralink_proto.dissector(buffer,pinfo,tree)
    pinfo.cols.protocol = "CAMERALINK"
    local subtree = tree:add(cameralink_proto,buffer(),"CameraLink Protocol Data")
    local msg_type = buffer(0,1):uint()
    
    if msg_type == 1 then 
        subtree:add(buffer(0,1),"TEST_MSG")
        pinfo.cols.info:append(" TEST_MSG")
    elseif msg_type == 2 then 
        subtree:add(buffer(0,1),"TEST_ACK")
        pinfo.cols.info:append(" TEST_ACK")
        local testresult = buffer(1,1):uint()
	subtree:add(buffer(0,1),"Auto test result: " .. testresult)
    elseif msg_type == 3 then
	subtree:add(buffer(0,1),"CONF_REQ")
        pinfo.cols.info:append(" CONF_REQ")
	local rHor = buffer(1,4):uint()
	local rVer = buffer(5,4):uint()
	subtree:add(buffer(0,1),"Resolution: ".. rHor .. "x" .. rVer)
	local focalLength = buffer(9,4):float()
	subtree:add(buffer(0,1),"focalLength: ".. focalLength)
	local interval = buffer(13,4):float()
	subtree:add(buffer(0,1),"Interval: ".. interval)
	local jitter = buffer(17,4):float()
	subtree:add(buffer(0,1),"Jitter: ".. jitter)
    elseif msg_type == 4 then
        subtree:add(buffer(0,1),"CONF_ACK")
        pinfo.cols.info:append(" CONF_ACK")
        local testresult = buffer(1,1):uint()
	subtree:add(buffer(0,1),"Auto test result: " .. testresult)
	
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
