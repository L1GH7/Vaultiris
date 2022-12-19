from django.core.cache import cache
from django.test import TestCase


class CacheTests(TestCase):
    def test_cache(self):
        cache.set('k1', 'v1')
        cache.set('k2', 'v2')

        try:
            cache.set('a b', 'v3')
        except Exception:
            pass

        self.assertEqual(
            [
                cache.get('k2'),
                cache.get('k1'),
                cache.get('k2'),
                cache.get('k1'),
                cache.get('k2'),
                cache.get('k1'),
            ],
            [
                'v2',
                'v1',
                'v2',
                'v1',
                'v2',
                'v1',
            ],
        )
