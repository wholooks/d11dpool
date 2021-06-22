from hashlib import sha256


class Share:
    def __init__(self, *, source, heads, env):
        self.timestamp = env.now
        self.source = source
        self.heads = heads
        self.hash = self.get_hash()

    def get_hash(self):
        joined_heads = "".join(self.heads)
        return sha256(f'{self.timestamp}{self.source}{joined_heads}'.
                      encode()).hexdigest()

    def __repr__(self):
        return f't: {self.timestamp} s: {self.source} {self.hash}'
