local P = {}

P.name = [=[�N���b�v�{�[�h����\��t��]=]

function P.oninitmenu()
  return P.name
end

function P.onselect(index, state)
  local files = GCMZDrops.getclipboard()
  if files == nil then
    return nil
  end
  return files, state
end

return P
