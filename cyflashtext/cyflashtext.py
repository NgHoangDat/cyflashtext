import re
from typing import *

from cyvi import Node
from pydantic import BaseModel


class Match(BaseModel):
    start: int
    end: int
    text: str
    meta: Optional[Dict[str, Any]]



class KeywordProcessor:

    def __init__(self, case_sensitive:bool):
        self.node = Node()
        self.data = {}

        self._case_sensitive = case_sensitive

    @property
    def case_sensitive(self):
        return self._case_sensitive

    def normalize(self, text:str, case_sensitive:bool) -> str:
        text = re.sub("\s+", " ", text).strip()
        if not case_sensitive:
            text = text.lower()

        return text

    def put(self, keyword:str, **kwargs) -> bool:
        keyword = self.normalize(keyword, self.case_sensitive)
    
        if keyword not in self.data:
            self.data[keyword] = kwargs
            self.node.put(keyword)
            return True
        
        return False

    def pop(self, keyword:str) -> Optional[Dict[str, Any]]:
        keyword = self.normalize(keyword, self.case_sensitive)

        if keyword in self.data:
            self.node.pop(keyword)
        
        return self.data.get(keyword, None)

    def extract(self, text:str, strict:bool=False) -> List[Match]:
        text = self.normalize(text, self.case_sensitive)
        return [
            Match(start=match.start, end=match.end, text=match.text, meta=self.data.get(match.text))
            for match in self.node.extract(text, strict)
        ]

    def replace(self, text:str, repl:Dict[str, str]) -> str:
        text = self.normalize(text, True)
        matches = self.extract(text)
        tokens = []
        curr = 0

        for match in matches:
            if curr < match.start:
                tokens.append(text[curr:match.start])

            tokens.append(repl.get(match.text, text[match.start:match.end]))

            curr = match.end

        if curr < len(text):
            tokens.append(text[curr:])

        return "".join(tokens)
